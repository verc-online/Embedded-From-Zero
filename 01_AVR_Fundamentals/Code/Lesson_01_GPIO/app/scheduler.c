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

#define  SCHEDULER_SECONDS_TICKS 61

static uint8_t lastCheckedMinute;

static FeedingTime feedingSchedule[] =
{
	{16, 30},
	{16, 31},
	{16, 31},
	{16, 32},
	{16, 33}
};

// TODO: Вернуть static
void Scheduler_SaveFeedingTime(uint16_t address,
const FeedingTime *time)
{
	// Получаем указатель на первый элемент в нашем расписании
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

// TODO: Вернуть static
void Scheduler_LoadFeedingTime(uint16_t address,											FeedingTime *time)
{
	uint8_t *ptr = (uint8_t*)time;
	for (uint8_t i = 0; i < sizeof(*time); i++)
	{
		ptr[i] = EEPROM_ReadByte(address + i);
	}
}


static void Scheduler_CheckFeedingTime(uint8_t currentHours, uint8_t currentMinutes)
{
	uint8_t scheduleCount = sizeof(feedingSchedule) / sizeof(feedingSchedule[0]);
	for (uint8_t i = 0; i < scheduleCount; i++)
	{
		if((feedingSchedule[i].hours == currentHours) && (feedingSchedule[i].minutes == currentMinutes))
		{
			Feeder_Request();
			break;
		}
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