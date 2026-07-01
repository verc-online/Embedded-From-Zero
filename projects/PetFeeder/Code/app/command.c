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

static bool Command_ParseDelete(char * command, uint8_t * index)
{
		// Указатель на текущий токен (часть строки)
		char *token;

		// Получаем первый токен
		// Для строки:
		// "delete 20"
		// token -> "delete"
		token = strtok(command, " ");

		if (token == NULL)
		{
			return false;
		}

		// Проверяем, что это действительно команда delete
		if (strcmp(token, "delete") != 0)
		{
			return false;
		}

		// Получаем второй токен
		// token -> "1"
		token = strtok(NULL, " ");

		if (token == NULL)
		{
			return false;
		}


		// Сохраняем результат, проверять уже будем внутри scheduler, потому что command.c не должен ничего знать про кормления
		*index = (uint8_t)atoi(token);

		// Проверяем, есть ли следующий токен, мы ожидаем, что нет
		token = strtok(NULL, " ");
		return token == NULL;
}

static bool Command_ParseSetTime(char * command, uint8_t* hours, uint8_t* minutes, uint8_t* seconds)
{
	// Указатель на текущий токен (часть строки)
	char *token;

	// Получаем первый токен
	// Для строки:
	// "settime 20 30"
	// token -> "settime"
	token = strtok(command, " ");

	if (token == NULL)
	{
		return false;
	}

	// Проверяем, что это действительно команда settime
	if (strcmp(token, "settime") != 0)
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
	
	// Получаем четвертый токен
	// token -> "30"
	token = strtok(NULL, " ");

	// Преобразуем строку в число
	int parsedSeconds = atoi(token);

	// Проверяем диапазон минут
	if ((parsedSeconds < 0) || (parsedSeconds > 59))
	{
		return false;
	}

	// Сохраняем результат
	*seconds = (uint8_t)parsedSeconds;

	// Проверяем, что больше параметров нет
	// Команда:
	// "settime 20 30 10"
	// пройдёт
	//
	// Команда:
	// "settime 20 30 10 abc"
	// не пройдёт
	token = strtok(NULL, " ");

	return token == NULL;
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
	
	if (strncmp(command, "settime ", 8) == 0)
	{
		uint8_t hours;
		uint8_t minutes;
		uint8_t seconds;
		if (Command_ParseSetTime(command, &hours, &minutes, &seconds))
		{
			RtcTime time = {hours, minutes, seconds};
			if (DS3231_SetTime(&time))
			{
				UART_SendLine("Time set");
			}
			else
			{
				UART_SendLine("Failed to set time");
			}
			if (DS3231_ReadTime(&time))
			{
				UART_SendTime(time.hours, time.minutes, time.seconds);
				UART_SendString("\r\n");
			}
			return;
		}
	}

	if (strncmp(command, "add ", 4) == 0)
	{
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
			Scheduler_PrintSchedule();
			return;
		}
	}
	
	if (strncmp(command, "delete ", 7) == 0)
	{
		uint8_t index;

		if (Command_ParseDelete(command, &index))
		{
			if ((index > 0) && Scheduler_DeleteFeedingTime(index - 1))
			{
				UART_SendLine("Feeding time deleted");
			}
			else
			{
				UART_SendLine("Failed to delete feeding time");
			}
			Scheduler_PrintSchedule();
			return;
		}
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