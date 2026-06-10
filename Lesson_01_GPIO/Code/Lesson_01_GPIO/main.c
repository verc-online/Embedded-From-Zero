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
#include "drivers/button.h"


int main(void)
{
	Led_Init();
	Button_Init();
	
    while (1) 
    {
		if(Button_IsPressed())
		{
			Led_On();
		}
		else
		{
			Led_Off();
		}
    }
}

