/*
* scheduler.h
*
* Created: 12.06.2026 10:33:31
*  Author: HP
*/


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
} FeedingTime;

void Scheduler_Init(void);
void Scheduler_Process(void);

// TODO: Убрать их отсюда и вернуть в .c static функциям
void Scheduler_SaveFeedingTime(uint16_t address, const FeedingTime *time);
void Scheduler_LoadFeedingTime(uint16_t address, FeedingTime *time);

#endif /* SCHEDULER_H_ */