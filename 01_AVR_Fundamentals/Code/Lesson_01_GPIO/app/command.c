/*
* command.c
*
* Created: 16.06.2026 20:40:03
*  Author: HP
*/
#define COMMAND_BUFFER_SIZE 32
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "command.h"
#include "../drivers/uart.h"
#include "debug.h"
#include "../drivers/ds3231.h"
#include "scheduler.h"

static char commandBuffer[COMMAND_BUFFER_SIZE];
static uint8_t commandIndex = 0;

static bool Command_ParseAdd(char *command,
uint8_t *hours,
uint8_t *minutes)
{
	// Указатель на текущий токен (часть строки)
	char *token;

	// Получаем первый токен
	// Для строки:
	// "add 20 30"
	// token -> "add"
	token = strtok(command, " ");

	if (token == NULL)
	{
		return false;
	}

	// Проверяем, что это действительно команда add
	if (strcmp(token, "add") != 0)
	{
		return false;
	}

	// Получаем второй токен
	// token -> "20"
	token = strtok(NULL, " ");

	if (token == NULL)
	{
		return false;
	}

	// Преобразуем строку в число
	int parsedHours = atoi(token);

	// Проверяем диапазон часов
	if ((parsedHours < 0) || (parsedHours > 23))
	{
		return false;
	}

	// Сохраняем результат
	*hours = (uint8_t)parsedHours;

	// Получаем третий токен
	// token -> "30"
	token = strtok(NULL, " ");

	if (token == NULL)
	{
		return false;
	}

	// Преобразуем строку в число
	int parsedMinutes = atoi(token);

	// Проверяем диапазон минут
	if ((parsedMinutes < 0) || (parsedMinutes > 59))
	{
		return false;
	}

	// Сохраняем результат
	*minutes = (uint8_t)parsedMinutes;

	// Проверяем, что больше параметров нет
	// Команда:
	// "add 20 30"
	// пройдёт
	//
	// Команда:
	// "add 20 30 abc"
	// не пройдёт
	token = strtok(NULL, " ");

	return token == NULL;
}

void Command_Init(void)
{

	commandIndex = 0;
	commandBuffer[0] = '\0';
	DEBUG_LOG("Command initialized");
}

static void Command_Execute(char *command)
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
	if (strcmp(command, "default") == 0)
	{
		if (Scheduler_LoadDefaultSchedule())
		{
			UART_SendLine("Default schedule loaded");
		}
		else
		{
			UART_SendLine("Failed to load default schedule");
		}
		Scheduler_PrintSchedule();
		return;
	}
	if (strcmp(command, "help") == 0)
	{
		UART_SendLine("Available commands:");
		UART_SendLine("help");
		UART_SendLine("time");
		UART_SendLine("show");
		UART_SendLine("default");
		UART_SendLine("add HH MM");
		return;
	}
	uint8_t hours;
	uint8_t minutes;

	if (Command_ParseAdd(command, &hours, &minutes))
	{
		if (Scheduler_AddFeedingTime(hours, minutes))
		{
			UART_SendLine("Feeding time added");
		}
		else
		{
			UART_SendLine("Failed to add feeding time");
		}

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