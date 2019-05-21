#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <mag/uart.h>
#include <stddef.h>

void usart_setup()
{
    // For the peripheral to work, we need to enable it's clock
    rcc_periph_clock_enable(RCC_USART1);
    // From the datasheet for the STM32F0 series of chips (Page 30, Table 11)
    // we know that the USART1 peripheral has it's TX line connected as
    // alternate function 1 on port A pin 9.
    // In order to use this pin for the alternate function, we need to set the
    // mode to GPIO_MODE_AF (alternate function). We also do not need a pullup
    // or pulldown resistor on this pin, since the peripheral will handle
    // keeping the line high when nothing is being transmitted.
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
    // Now that we have put the pin into alternate function mode, we need to
    // select which alternate function to use. PA9 can be used for several
    // alternate functions - Timer 15, USART1 TX, Timer 1, and on some devices
    // I2C. Here, we want alternate function 1 (USART1_TX)
    gpio_set_af(GPIOA, GPIO_AF1, GPIO9);
    // Now that the pins are configured, we can configure the USART itself.
    // First, let's set the baud rate at 115200
    usart_set_baudrate(USART1, 115200);
    // Each datum is 8 bits
    usart_set_databits(USART1, 8);
    // No parity bit
    usart_set_parity(USART1, USART_PARITY_NONE);
    // One stop bit
    usart_set_stopbits(USART1, USART_CR2_STOPBITS_1);
    // For a debug console, we only need unidirectional transmit
    usart_set_mode(USART1, USART_MODE_TX);
    // No flow control
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    // Enable the peripheral
    usart_enable(USART1);

    // Optional extra - disable buffering on stdout.
    // Buffering doesn't save us any syscall overhead on embedded, and
    // can be the source of what seem like bugs.
    //setbuf(stdout, NULL);
}



void uart_puts(char *string) {
    while (*string) {
        usart_send_blocking(USART1, *string);
        string++;
    }
}

void uart_putln(char *string) {
    uart_puts(string);
    uart_puts("\r\n");
}
