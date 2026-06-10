/*
* button.c
*
* Created: 10.06.2026 17:08:41
*  Author: HP
*/

#include <avr/io.h>
#include <stdbool.h>
#include "button.h"
#include "../config/config.h"


void Button_Init(void)
{
	// Настраиваем пин кнопки как вход
	DDRD &= ~(1 << BUTTON_PIN);
	
	//Включаем внутреннуюю подтяжку Pull-Up
	PORTD |= (1 << BUTTON_PIN);
}
bool Button_IsPressed(void)
{
	// Кнопка подключена к земле, поэтому при нажатии читаем 0
	return !(PIND &(1 << BUTTON_PIN));
}

