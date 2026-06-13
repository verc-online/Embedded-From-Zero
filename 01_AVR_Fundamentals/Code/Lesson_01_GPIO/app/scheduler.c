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

#define  SCHEDULER_SECONDS_TICKS 61

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
} FeedingTime;

static uint8_t lastCheckedMinute;

static FeedingTime feedingSchedule[] =
{
	{16, 30},
	{16, 31},
	{16, 31},
	{16, 32},
	{16, 33}
};

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