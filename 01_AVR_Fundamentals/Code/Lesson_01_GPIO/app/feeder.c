/*
* feeder.c
*
* Created: 11.06.2026 10:40:12
*  Author: HP
*/
#include <stdint.h>
#include "../config/config.h"
#include "../app/feeder.h"
#include "../drivers/timer.h"
#include "../drivers/servo.h"
#include "../app/debug.h"

static FeederState feederState = FEEDER_IDLE;
static uint32_t stateStartTime = 0;

void Feeder_Init(void)
{
	feederState = FEEDER_IDLE;
	Servo_SetAngle(FEEDER_GATE_CLOSED_ANGLE);
}
void Feeder_Request(void)
{
	DEBUG_LOG("Feeder Request");
	if (feederState != FEEDER_IDLE) return;
	
	Servo_SetAngle(FEEDER_GATE_OPEN_ANGLE);
	DEBUG_LOG("Servo Open");
	
	stateStartTime = Timer_GetTicks();
	
	feederState = FEEDER_WAITING;
}

void Feeder_Process(void)
{
	switch(feederState)
	{
		case FEEDER_IDLE: break;
		
		case FEEDER_WAITING:
		if(Timer_HasElapsed(stateStartTime, 120))
		{
			Servo_SetAngle(FEEDER_GATE_CLOSED_ANGLE);
			DEBUG_LOG("Servo Close");
			feederState = FEEDER_IDLE;
		}
		break;
		
		default: break;
	}
}

FeederState Feeder_GetState(void)
{
	return feederState;
}