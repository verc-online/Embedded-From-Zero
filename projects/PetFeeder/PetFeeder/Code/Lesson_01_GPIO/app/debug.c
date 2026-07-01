/*
 * debug.c
 *
 * Created: 13.06.2026 12:00:30
 *  Author: HP
 */ 

#include "debug.h"
#include "../drivers/uart.h"
#include "../drivers/ds3231.h"

void Debug_Log(const char *message)
{
	RtcTime time;

	if (DS3231_ReadTime(&time))
	{
		UART_SendChar('[');
		UART_SendTime(time.hours, time.minutes, time.seconds);
		UART_SendString("] ");
	}
	else
	{
		UART_SendString("[NO RTC] ");
	}

	UART_SendString(message);
	UART_SendString("\r\n");
}