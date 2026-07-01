/*
 * servo.c
 *
 * Created: 11.06.2026 15:13:30
 *  Author: HP
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "servo.h"

#define SERVO_MIN_TICKS 125
#define SERVO_CENTER_TICKS 188
#define SERVO_MAX_TICKS 250
#define SERVO_PERIOD_TICKS 2500

void Servo_Init(void)
{
	// PB1 = OC1A, выход аппаратного PWM Timer1
	DDRB |= (1 << PB1);

	// Fast PWM, TOP = ICR1
	TCCR1A = 0;
	TCCR1B = 0;

	// Non-inverting mode on OC1A
	TCCR1A |= (1 << COM1A1);

	// Fast PWM mode 14: WGM13:0 = 1110
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM13) | (1 << WGM12);

	// Период 20 мс
	ICR1 = SERVO_PERIOD_TICKS;

	// Центр сервы
	OCR1A = SERVO_CENTER_TICKS;

	// Prescaler = 8
	TCCR1B |= (1 << CS11);
}

void Servo_SetAngle(uint8_t angle)
{
	if (angle > 180)
	{
		angle = 180;
	}

	uint16_t ticks = SERVO_MIN_TICKS +
	((uint32_t)angle * (SERVO_MAX_TICKS - SERVO_MIN_TICKS)) / 180;

	OCR1A = ticks;
}
