/*
 * feeder.c
 *
 * Created: 11.06.2026 10:40:12
 *  Author: HP
 */ 

#include "feeder.h"
#include "../drivers/led.h"


void Feeder_Init(void)
{
	// Здесь пока ничего.
	// Позже будет инициализация сервопривода, мотора, датчиков	
}
void Feeder_Request(void)
{
	// Пока Led имитирует выдачку корма
	// Позже здесь будет Servo_Open(), Motor_Run(), Servo_Close()
	Led_Blink(5);
}