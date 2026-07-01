/*
 * led.c
 *
 * Created: 10.06.2026 11:19:58
 *  Author: HP
 */ 
#include "../config/config.h"
#include <avr/io.h>
#include <util/delay.h>

#include "led.h"


void Led_Init(void)
{
	DDRB |= (1 << LED_PIN);
	PORTB &= ~(1 << LED_PIN);
}

void Led_On(void)
{
	PORTB |= (1 << LED_PIN);
}

void Led_Off(void)
{
	PORTB &= ~(1 << LED_PIN);
}

void Led_Toggle()
{
	PORTB ^= (1 << LED_PIN);
}

void Led_Blink(uint8_t count)
{
	for (uint8_t i = 0; i < count; i++)
	{
		Led_On();
		_delay_ms(200);
		Led_Off();
		_delay_ms(200);
	}
}