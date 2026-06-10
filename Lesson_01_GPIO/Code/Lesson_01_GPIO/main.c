/*
 * Lesson_01_GPIO.c
 *
 * Created: 10.06.2026 11:19:09
 * Author : HP
 */ 
#include "config/config.h"
#include <avr/io.h>
#include <util/delay.h>
#include "drivers/led.h"

int main(void)
{
	Led_Init();
    /* Replace with your application code */
    while (1) 
    {
		Led_Blink(3);
		_delay_ms(1000);
    }
}

