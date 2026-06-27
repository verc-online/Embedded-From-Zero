/*
* scheduler.h
*
* Created: 12.06.2026 10:33:31
*  Author: HP
*/


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
} FeedingTime;


void Scheduler_Init(void);
void Scheduler_Process(void);
bool Scheduler_LoadSettings(void);
bool Scheduler_SaveSettings(void);
void Scheduler_PrintSchedule(void);
bool Scheduler_AddFeedingTime(uint8_t hours, uint8_t minutes);
bool Scheduler_LoadDefaultSchedule(void);
bool Scheduler_DeleteFeedingTime(uint8_t index);
uint8_t Scheduler_GetFeedingCount(void);
bool Scheduler_GetFeedingTime(uint8_t index, FeedingTime *time);

#endif /* SCHEDULER_H_ */