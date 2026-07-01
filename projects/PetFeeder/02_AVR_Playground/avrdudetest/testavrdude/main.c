/*
 * testavrdude.c
 *
 * Created: 29.06.2026 21:20:53
 * Author : HP
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1 << PB0);

	while (1)
	{
		PORTB ^= (1 << PB0);
		_delay_ms(500);
	}
}
