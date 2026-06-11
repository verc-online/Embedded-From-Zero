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
#include "app/feeder.h"

int main(void)
{
	Led_Init();
	Button_Init();
	bool previousState = false;
	
    while (1) 
    {
		bool currentState = Button_IsPressedDebounced();
		
		if((previousState == false) && (currentState == true))
		{
			Feeder_Request();
		}
		
		previousState = currentState;
    }
}

