/*
* uart.c
*
* Created: 13.06.2026 9:35:43
*  Author: HP
*/
#include <stdint.h>
#include <avr/io.h>
#include "uart.h"

void UART_Init(void)
{
	// 9600 baud при F_CPU = 1 MHz, U2X = 1
	UBRRH = 0;
	UBRRL = 12;

	// Double Speed Mode
	UCSRA |= (1 << U2X);

	// ¬ключаем только передачу UART
	UCSRB = (1 << TXEN);

	// UCSRC и UBRRH имеют общий адрес в ATmega8.
	// URSEL = 1 означает: записываем именно в UCSRC.
	// UCSZ1 = 1 и UCSZ0 = 1 ? 8 бит данных.
	UCSRC = (1 << URSEL)
	| (1 << UCSZ1)
	| (1 << UCSZ0);
}

void UART_SendChar(char c)
{
	while(!(UCSRA & (1 << UDRE)))
	{
		// ∆дЄм освобождени€ регистра UDR
	}
	UDR = c;
}

void UART_SendString(const char *str)
{
	while (*str != '\0')
	{
		UART_SendChar(*str);
		str++;
	}
}

void UART_SendNumber(uint16_t value)
{
	char buffer[6];
	uint8_t index = 5;

	buffer[index] = '\0';

	if (value == 0)
	{
		UART_SendChar('0');
		return;
	}

	while (value > 0)
	{
		uint8_t digit = value % 10;
		index--;
		buffer[index] = digit + '0';
		value /= 10;
	}

	UART_SendString(&buffer[index]);
}