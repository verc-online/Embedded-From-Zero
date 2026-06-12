/*
 * scheduler.c
 *
 * Created: 12.06.2026 10:33:39
 *  Author: HP
 */ 

#include "scheduler.h"
#include "feeder.h"
#include "../drivers/timer.h"

#define  SCHEDULER_SECONDS_TICKS 61

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
} FeedingTime;

static uint8_t hours = 7;
static uint8_t minutes = 59;
static uint8_t seconds = 50;

static uint32_t lastSecondTime = 0;

static FeedingTime feedingSchedule[] =
{
	{8, 0},
	{20, 0}
};

static void Scheduler_UpdateTime (bool *minuteChanged)
{
	*minuteChanged = false;
	
	seconds++;
	
	if (seconds >= 60)
	{
		seconds = 0;
		minutes++;
		*minuteChanged = true;
	}
	
	if (minutes >= 60)
	{
		minutes = 0;
		hours++;
	}
	
	if(hours >= 24)
	{
		hours = 0;
	}
}

static void Scheduler_CheckFeedingTime(void)
{
	uint8_t scheduleCount = sizeof(feedingSchedule) / sizeof(feedingSchedule[0]);
	for (uint8_t i = 0; i < scheduleCount; i++)
	{
		if((feedingSchedule[i].hours == hours) && (feedingSchedule[i].minutes == minutes))
		{
			Feeder_Request();
			break;
		}
	}
}

void Scheduler_Init(void)
{
	hours = 7;
	minutes = 59;
	seconds = 50;
	
	lastSecondTime = Timer_GetTicks();
}

void Scheduler_Process(void)
{
	if (!Timer_HasElapsed(lastSecondTime, SCHEDULER_SECONDS_TICKS))
	{
		return;
	}
	lastSecondTime = Timer_GetTicks();
	bool minuteChanged = false;
	Scheduler_UpdateTime(&minuteChanged);
	
	if(minuteChanged)
	{
		Scheduler_CheckFeedingTime();
	}
}
