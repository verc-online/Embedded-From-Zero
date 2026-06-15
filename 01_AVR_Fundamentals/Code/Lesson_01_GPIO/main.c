/*
 * Lesson_01_GPIO.c
 *
 * Created: 10.06.2026 11:19:09
 * Author : HP
 */ 
#include "config/config.h"
#include <avr/io.h>
#include "drivers/led.h"
#include "drivers/button.h"
#include "drivers/timer.h"
#include "app/feeder.h"
#include "drivers/servo.h"
#include "drivers/ds3231.h"
#include "drivers/twi.h"
#include "app/scheduler.h"
#include "drivers/uart.h"
#include "app/debug.h"
#include "drivers/eeprom.h"

int main(void)
{
	Led_Init();
	Button_Init();
	Timer_Init();
	Servo_Init();
	TWI_Init();
	UART_Init();
	
	Feeder_Init();
	Scheduler_Init();

	
	bool previousState = false;
	
	DEBUG_LOG("System start");
	
	// TODO: Удалить и вернуть функциям static
	FeedingTime testSave = {16, 30};
	FeedingTime testLoad = {0, 0};

	Scheduler_SaveFeedingTime(20, &testSave);
	Scheduler_LoadFeedingTime(20, &testLoad);

	UART_SendString("Loaded time: ");
	UART_SendNumber2(testLoad.hours);
	UART_SendChar(':');
	UART_SendNumber2(testLoad.minutes);
	UART_SendString("\r\n");
	// =========================================
	
    while (1) 
    {
		bool currentState = Button_IsPressedDebounced();
		
		if((previousState == false) && (currentState == true))
		{
			DEBUG_LOG("Scheduler match");
			Feeder_Request();
		}
		previousState = currentState;
		
		Scheduler_Process();
		Feeder_Process();
    }
}

