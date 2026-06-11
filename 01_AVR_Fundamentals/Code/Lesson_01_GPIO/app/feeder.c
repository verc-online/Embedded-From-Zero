/*
* feeder.c
*
* Created: 11.06.2026 10:40:12
*  Author: HP
*/
#include <stdint.h>
#include "feeder.h"
#include "../drivers/led.h"

static FeederState feederState = FEEDER_IDLE;
static uint16_t processCounter = 0;
static uint8_t blinkCount = 0;

void Feeder_Init(void)
{
	feederState = FEEDER_IDLE;
	processCounter = 0;
	blinkCount = 0;
}
void Feeder_Request(void)
{
	if (feederState != FEEDER_IDLE) return;
	feederState = FEEDER_FEEDING;
	processCounter = 0;
	blinkCount = 0;
}

void Feeder_Process(void)
{
	if (feederState == FEEDER_IDLE) return;
	
	if(feederState == FEEDER_FEEDING)
	{
		processCounter++;
		if (processCounter >= 5000)
		{
			Led_Toggle();
			processCounter = 0;
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