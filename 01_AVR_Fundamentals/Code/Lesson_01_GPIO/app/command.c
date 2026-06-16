/*
 * command.c
 *
 * Created: 16.06.2026 20:40:03
 *  Author: HP
 */ 
#define COMMAND_BUFFER_SIZE 32

#include <string.h>
#include "command.h"
#include "../drivers/uart.h"
#include "debug.h"
#include "../drivers/ds3231.h"
#include "scheduler.h"

static char commandBuffer[COMMAND_BUFFER_SIZE];
static uint8_t commandIndex = 0;

void Command_Init(void)
{

	commandIndex = 0;
	commandBuffer[0] = '\0';
	DEBUG_LOG("Command initialized");
}

static void Command_Execute(const char *command)
{
	if (strcmp(command, "time") == 0)
	{
		RtcTime time;

		if (DS3231_ReadTime(&time))
		{
			UART_SendTime(time.hours, time.minutes, time.seconds);
			UART_SendString("\r\n");
		}
		else
		{
			UART_SendString("RTC Error\r\n");
		}

		return;
	}
	if (strcmp(command, "show") == 0)
	{
		Scheduler_PrintSchedule();
		return;
	}

	UART_SendString("Unknown command: ");
	UART_SendString(command);
	UART_SendString("\r\n");
}

void Command_Process(void)
{
	if (!UART_IsCharAvailable())
	{
		/*DEBUG_LOG("Char is UnAvailable");*/
		return;
	}
	char c = UART_ReadChar();
	if ((c == '\r') || (c == '\n'))
	{
		if (commandIndex == 0)
		{
			return;
		}
		commandBuffer[commandIndex] = '\0';

		Command_Execute(commandBuffer);

		commandIndex = 0;
		return;
	}

	if (commandIndex < COMMAND_BUFFER_SIZE - 1)
	{
		commandBuffer[commandIndex] = c;
		commandIndex++;
	}
}