/*
 * feeder.h
 *
 * Created: 11.06.2026 10:40:19
 *  Author: HP
 */ 


#ifndef FEEDER_H_
#define FEEDER_H_


typedef enum
{
	FEEDER_IDLE,

	FEEDER_OPENING,

	FEEDER_WAITING,

	FEEDER_CLOSING,

	FEEDER_NO_FOOD,

	FEEDER_ERROR

} FeederState;


void Feeder_Init(void);
void Feeder_Request(void);
void Feeder_Process(void);
FeederState Feeder_GetState(void);

#endif /* FEEDER_H_ */