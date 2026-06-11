/*
* feeder.c
*
* Created: 11.06.2026 10:40:12
*  Author: HP
*/
#include <stdint.h>
#include "feeder.h"
#include "../drivers/led.h"
#include "../drivers/timer.h"

static FeederState feederState = FEEDER_IDLE;
static uint32_t lastBlinkTime  = 0;
static uint8_t blinkCount = 0;

void Feeder_Init(void)
{
	feederState = FEEDER_IDLE;
	lastBlinkTime = 0;
	blinkCount = 0;
}
void Feeder_Request(void)
{
	if (feederState != FEEDER_IDLE) return;
	feederState = FEEDER_FEEDING;
	lastBlinkTime = Timer_GetTicks();
	blinkCount = 0;
}

void Feeder_Process(void)
{
	if (feederState == FEEDER_IDLE) return;
	
	if(feederState == FEEDER_FEEDING)
	{
		if (Timer_HasElapsed(lastBlinkTime, 30))
		{
			Led_Toggle();
			lastBlinkTime = Timer_GetTicks();
			blinkCount++;
		}
		
		if(blinkCount >= 10)
		{
			Led_Off();
			feederState = FEEDER_IDLE;
		}
	}
}

FeederState Feeder_GetState(void)
{
	return feederState;
}