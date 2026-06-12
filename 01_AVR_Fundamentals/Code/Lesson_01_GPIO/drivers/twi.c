/*
* twi.c
*
* Created: 12.06.2026 12:33:48
*  Author: HP
*/
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#include "../config/config.h"
#include "twi.h"

static uint8_t TWI_GetStatus(void)
{
	// Маска 0xF8 = 11111000, чтобы не трогать младшие биты, которые отвечают за Prescaler
	return TWSR & 0xF8;
}

void TWI_Init(void)
{
	// Prescaler = 1
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
	
	// SCL ? 50 kHz при F_CPU = 1 MHz
	TWBR = 2;
	
	// Включаем TWI
	TWCR = (1 << TWEN);
}

bool TWI_Start(void)
{
	TWCR = (1 << TWINT)
	| (1<< TWSTA)
	| (1 << TWEN);
	while (!(TWCR & (1 << TWINT)))
	{
	}
	
	// TWSR 0x08 - означает :
	// 0x08 = START condition has been transmitted
	if (TWI_GetStatus() != 0x08)
	{
		return false;
	}

	return true;
}

bool TWI_Write(uint8_t data, uint8_t expectedStatus)
{
	TWDR = data;

	TWCR = (1 << TWINT)	| (1 << TWEN);

	while (!(TWCR & (1 << TWINT)))
	{
	}

	if (TWI_GetStatus() != expectedStatus)
	{
		return false;
	}

	return true;
}

void TWI_Stop(void)
{
	TWCR = (1 << TWINT)
	| (1 << TWSTO)
	| (1 << TWEN);
}

uint8_t TWI_ReadAck(void)
{
	TWCR = (1 << TWINT)
	| (1 << TWEN)
	| (1 << TWEA); // после приёма байта отправить ACK

	while (!(TWCR & (1 << TWINT)))
	{
	}

	return TWDR;
}

uint8_t TWI_ReadNack(void)
{
	TWCR = (1 << TWINT)
	| (1 << TWEN);
	// TWEA = 0 после приёма байта отправить NACK
	while (!(TWCR & (1 << TWINT)))
	{
	}

	return TWDR;
}