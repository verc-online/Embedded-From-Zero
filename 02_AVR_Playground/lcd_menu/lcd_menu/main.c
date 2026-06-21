/*
 * lcd_menu.c
 *
 * Created: 20.06.2026 13:33:27
 * Author : HP
 */ 

#include <avr/io.h>
#include "config/config.h"
#include "drivers/lcd1602.h"

int main(void)
{
	LCD_Init();

	LCD_SetCursor(0, 0);
	LCD_Print("Hello Kisimisi!");
	
	LCD_SetCursor(1, 0);
	LCD_Print("I love you <3");
	
    while (1) 
    {
    }
}

