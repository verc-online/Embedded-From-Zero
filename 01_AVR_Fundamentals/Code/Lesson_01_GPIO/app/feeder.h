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
	FEEDER_IDLE,      // ожидание
	FEEDER_FEEDING,   // выдача корма
	FEEDER_NO_FOOD,   // нет корма
	FEEDER_ERROR     // ошибка
} FeederState;


void Feeder_Init(void);
void Feeder_Request(void);
void Feeder_Process(void);
FeederState Feeder_GetState(void);

#endif /* FEEDER_H_ */