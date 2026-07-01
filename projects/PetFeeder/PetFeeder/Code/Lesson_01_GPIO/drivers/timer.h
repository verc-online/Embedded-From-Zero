/*
 * timer.h
 *
 * Created: 11.06.2026 11:53:51
 *  Author: HP
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <stdbool.h>

void Timer_Init(void);
uint32_t Timer_GetTicks(void);
bool Timer_HasElapsed(uint32_t startTime, uint32_t intervalMs);

#endif /* TIMER_H_ */