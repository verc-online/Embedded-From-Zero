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
#include "app/command.h"
#include "drivers/lcd1602.h"
#include "app/menu.h"

int main(void)
{
	Led_Init();
	Timer_Init();
	Servo_Init();
	TWI_Init();
	UART_Init();
	Feeder_Init();
	Scheduler_Init();
	Command_Init();
	LCD_Init();
	Menu_Init();
		
	
	DEBUG_LOG("System start");

    while (1) 
    {
		Scheduler_Process();
		Feeder_Process();
		Command_Process();
		Menu_Process();
    }
}

