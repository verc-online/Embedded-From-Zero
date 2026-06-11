/*
 * timer.c
 *
 * Created: 11.06.2026 11:53:42
 *  Author: HP
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

#include "timer.h"

static volatile uint32_t systemTicks = 0;

void Timer_Init(void)
{
	// Timer0 normal mode
	TCCR0 = 0x00;
	
	// Prescaler 64
	TCCR0 |= (1 << CS01) | (1 << CS00);
	
	// Enable Timer0 overflow interrupt
	TIMSK |= (1 << TOIE0);
	
	// Enbale global interrupts
	sei();
}

ISR(TIMER0_OVF_vect)
{
	systemTicks++;
}

uint32_t Timer_GetTicks(void)
{
	uint32_t ticks;
	
	cli();
	ticks = systemTicks;
	sei();
	
	return ticks;
}

bool Timer_HasElapsed(uint32_t startTime, uint32_t intervalMs)
{
	return (Timer_GetTicks() - startTime) >= intervalMs;
}