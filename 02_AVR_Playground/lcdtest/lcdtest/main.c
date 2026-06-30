/*
 * lcdtest.c
 *
 * Created: 30.06.2026 20:49:07
 * Author : HP
 */ 

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "config.h"
#include "lcd1602.h"

int main(void)
{
	LCD_Init();

	LCD_Clear();

	LCD_SetCursor(0, 0);
	LCD_Print("LCD OK");

	LCD_SetCursor(1, 0);
	LCD_Print("ATmega8 1MHz");

	while (1)
	{
	}
}
