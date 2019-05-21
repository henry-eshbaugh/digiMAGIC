#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

static void clock_setup(void)
{
	/* enable 25MHz crystal - high speed external (HSE) oscillator */
	rcc_clock_setup_in_hse_25mhz_out_72mhz();

	/* Enable GPIOA clock (for LED GPIOs). */
	/* rcc_periph_clock_enable(RCC_GPIOC); */

	/* Enable clocks for GPIO port A (for GPIO_USART2_TX) and USART2. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_USART2);

	/* Must also set RTC based on this external clock.     *
	 * Could register an interrupt? Or, just use RTC shizz */
}


static void usart_setup(void)
{
	/* check to see if a USB device is connected by sensing
	 * the state of PB2, USBSER_ISCONN */
	/* Setup GPIO pin GPIO_USART2_RE_TX on GPIO port B for transmit. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART2_TX);

	/* PA9  FTDI_TXD *
	 * PA10 FTDI_RXD *
	 * PA11 FTDI_RTS *
	 * PA12 FTDI_CTS */

	/* Setup UART parameters. */
	usart_set_baudrate(USART2, 230400);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_parity(USART2, USART_PARITY_NONE);
	/* todo: RTS, CTS */
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART2, USART_MODE_TX);

	/* Finally enable the USART. */
	usart_enable(USART2);
}


static void gpio_setup(void)
{
	/* INA: PC6, TIM8_CH1 */
	/* INB: PC7, TIM8_CH2 */
	/* set(), reset() need outputs enabled *
	 * on these pins to write 3 us pulses  */
	gpio_set(GPIOA, GPIO8);

	/* Setup GPIO8 (in GPIO port A) for LED use. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO8);
}

static void spi_setup(void)
{
	/* ADC: SPI3 *
	 * PC10 SCLK *
	 * PC11 MISO *
	 * PC12 MOSI */
	

	/* DAC: SPI2 *
	 * PB13 SCLK *
	 * PB14 MISO *
	 * PB15 MOSI */
	/* DAC also has nLDAC, nSYNC signals,
	 * which are on PB11 and PB12 respectively */
}

void boot(void)
{
	clock_setup();
	usart_setup();
	gpio_setup();
}
