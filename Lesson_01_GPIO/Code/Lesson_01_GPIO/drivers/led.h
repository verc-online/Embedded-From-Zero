/*
 * led.h
 *
 * Created: 10.06.2026 11:24:15
 *  Author: HP
 */ 


#ifndef LED_H_
#define LED_H_
#include <stdint.h>

void Led_Init(void);

void Led_On(void);

void Led_Off(void);

void Led_Toggle(void);

void Led_Blink(uint8_t count);

#endif /* LED_H_ */