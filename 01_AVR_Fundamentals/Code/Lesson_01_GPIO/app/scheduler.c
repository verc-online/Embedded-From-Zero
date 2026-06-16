/*
 * scheduler.c
 *
 * Created: 12.06.2026 10:33:39
 *  Author: HP
 */ 

#include "scheduler.h"
#include "feeder.h"
#include "../drivers/timer.h"
#include "../drivers/ds3231.h"
#include "../drivers/eeprom.h"
#include "../drivers/uart.h"

#include "debug.h"

#define  SCHEDULER_SECONDS_TICKS 61

#define EEPROM_MAGIC_VALUE      0xA6
#define EEPROM_MAGIC_ADDRESS    0
#define EEPROM_COUNT_ADDRESS    1
#define EEPROM_SCHEDULE_ADDRESS 2

#define MAX_FEEDING_COUNT_TIMES 5

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
} FeedingTime;

static uint8_t feedingCount = 0;
static FeedingTime feedingSchedule[MAX_FEEDING_COUNT_TIMES];
static const FeedingTime defaultSchedule[] = 
{
	{8, 0},
	{12, 0},
	{18, 0},
};
static uint8_t lastCheckedMinute;


static void Scheduler_SaveFeedingTime(uint16_t address,
const FeedingTime *time)
{
	// Представляем структуру FeedingTime как массив байтов
	const uint8_t *ptr = (const uint8_t*)time;
	
	// Получаем размер нашей структуры расписания
	for (uint8_t i = 0; i < sizeof(*time); i++)
	{
		// Сдвигаемся по регистру памяти на i элемент и получаем данные
		EEPROM_UpdateByte(address + i, ptr[i]);
		// Эта запись эквивалентна
		// EEPROM_UpdateByte(address + i, *(ptr + i));
		// Но компилятор делает это за нас 
		// ptr[i] == *(ptr + i)
		
		// И эта запись эквивалентна
		// EEPROM_UpdateByte(address + i, *ptr);
		// ptr++;
	}
}

static void Scheduler_LoadFeedingTime(uint16_t address,											FeedingTime *time)
{
	uint8_t *ptr = (uint8_t*)time;
	for (uint8_t i = 0; i < sizeof(*time); i++)
	{
		ptr[i] = EEPROM_ReadByte(address + i);
	}
}


static void Scheduler_CheckFeedingTime(uint8_t currentHours, uint8_t currentMinutes)
{

	for (uint8_t i = 0; i < feedingCount; i++)
	{
		if((feedingSchedule[i].hours == currentHours) && (feedingSchedule[i].minutes == currentMinutes))
		{
			Feeder_Request();
			break;
		}
	}
}



void Scheduler_Process(void)
{
	RtcTime currentTime;

	if (!DS3231_ReadTime(&currentTime))
	{
		return;
	}

	if (currentTime.minutes == lastCheckedMinute)
	{
		return;
	}

	lastCheckedMinute = currentTime.minutes;

	Scheduler_CheckFeedingTime(currentTime.hours, currentTime.minutes);
}

bool Scheduler_LoadSettings(void)
{
	if (EEPROM_ReadByte(EEPROM_MAGIC_ADDRESS) != EEPROM_MAGIC_VALUE)
	{
		return false;
	}
	feedingCount = EEPROM_ReadByte(EEPROM_COUNT_ADDRESS);
	
	if ((feedingCount == 0) || (feedingCount > MAX_FEEDING_COUNT_TIMES))
	{
		return false;
	}
	
	
	for (uint8_t i = 0; i < feedingCount; i++)
	{
		uint16_t address = EEPROM_SCHEDULE_ADDRESS + i * sizeof(FeedingTime);
		Scheduler_LoadFeedingTime(address, &feedingSchedule[i]);
	}
	
	return true;
}

bool Scheduler_SaveSettings(void)
{
	
	if ((feedingCount == 0) || (feedingCount > MAX_FEEDING_COUNT_TIMES))
	{
		return false;
	}
	
	EEPROM_UpdateByte(EEPROM_MAGIC_ADDRESS, EEPROM_MAGIC_VALUE);
	EEPROM_UpdateByte(EEPROM_COUNT_ADDRESS, feedingCount);
	
	for (uint8_t i = 0; i < feedingCount; i++)
	{
		uint16_t address = EEPROM_SCHEDULE_ADDRESS + i * sizeof(FeedingTime);
		Scheduler_SaveFeedingTime(address, &feedingSchedule[i]);
	}
	
	return true;
}


void Scheduler_PrintSchedule(void)
{
	UART_SendString("Schedule:\r\n");
	for (uint8_t i = 0; i < feedingCount; i++)
	{
		UART_SendNumber(i + 1);
 		UART_SendString(". ");

		UART_SendNumber2(feedingSchedule[i].hours);
		UART_SendChar(':');
		UART_SendNumber2(feedingSchedule[i].minutes);

		UART_SendString("\r\n");
	}
}

void Scheduler_Init(void)
{
	RtcTime currentTime;

	if (DS3231_ReadTime(&currentTime))
	{
		lastCheckedMinute = currentTime.minutes;
	}
	else
	{
		lastCheckedMinute = 255;
	}
	DEBUG_LOG("Initializing Schedule");
	if (!Scheduler_LoadSettings())
	{
		feedingCount = sizeof(defaultSchedule) / sizeof(defaultSchedule[0]);
		
		for (uint8_t i = 0; i < feedingCount; i++)
		{
			feedingSchedule[i] = defaultSchedule[i];
		}
		
		Scheduler_SaveSettings();
		DEBUG_LOG("Default Schedule Saved");
	
	}
}
