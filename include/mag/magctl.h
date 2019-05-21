#ifndef mag_ctl_h
#define mag_ctl_h

#include <stdint.h>
#include <inc/dac.h>

static inline set()
{
	gpio_write_high(SET_PIN);
	wait_us(5); // TODO
	gpio_write_low(SET_PIN);
}

extern void reset()
{

	gpio_write_high(RESET_PIN);
	wait_us(5); // TODO
	gpio_write_low(RESET_PIN);
}

static inline uint16_t fb_mask(uint32_t conv)
{ return (uint16_t) ((conv>>11) & 0xFFFF); }
static inline void feedback(uint16_t fb)
{
	dac_out(fb);
}

#endif // !defined(mag_ctl_h)
