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

void Scheduler_Init(void);
void Scheduler_Process(void);
bool Scheduler_LoadSettings(void);
bool Scheduler_SaveSettings(void);
void Scheduler_PrintSchedule(void);

#endif /* SCHEDULER_H_ */