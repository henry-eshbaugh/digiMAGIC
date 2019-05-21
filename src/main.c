#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

int main()
{
	// switch to external xtal
	rcc_clock_setup_in_hsi_out_48mhz();
	// this probably doesn't matter? -
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11);

	// initialize peripherals
	uart_setup();
	adc_setup();
	dac_setup();

	// begin magnetometer conversions
	while (1)
		mag_conv_and_write();

	return -1;
}
