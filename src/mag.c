#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <mag/uart.h>
#include <mag/magctrl.h>

/* lock into a square wave 2^lockin_decim times slower than         *
 * the sample rate, and filter/decimate a further 2^downsamp times. *
 * Flips the magnetometer with set()/reset(), thus generating the   *
 * square wave to lock into.                                        */

void mag_conv_and_write(uint16_t lockin_decim, uint16_t downsamp)
{
	printf(DBG_PKT "mag_conv_and_write()\n");
	int32_t magconv = 0, k = 0;
	static uint32_t backoff;

	for (int j = 0; j < downsamp; j++) {
		/* +ve sense measurement */
		set();
		for (int i = 0; i < lockin_decim; i++)
			k += mag_convert();

		/* -ve sense measurement */
		reset();
		for (int i = 0; i < lockin_decim; i++)
			k -= mag_convert();

		/* done with one 2ms period. Feed back and accumulate. */
		feedback(k);
		magconv += k;
		k = 0;
	}
	
	/* reading out over UART */
	printf(SCI_PKT "%d, %d", instrument_time(), magconv);
}

/* TODO: the feedback logic is wonky here.
 * need some IIR filter-like design. If we're locked into
   a small square wave */
