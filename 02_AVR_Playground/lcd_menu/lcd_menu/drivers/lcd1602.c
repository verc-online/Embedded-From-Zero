/*
* lcd1602.c
*
* Created: 20.06.2026 14:07:03
*  Author: HP
*/

#include <avr/io.h>
#include "../config/config.h"
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include "lcd1602.h"

static void LCD_WriteNibble(uint8_t nibble)
{
	if (nibble & (1 << 0))
	{
		LCD_D4_PORT |= (1 << LCD_D4_PIN);
	}
	else
	{
		LCD_D4_PORT &= ~(1 << LCD_D4_PIN);
	}
	
	if (nibble & (1 << 1))
	{
		LCD_D5_PORT |= (1 << LCD_D5_PIN);
	}
	else
	{
		LCD_D5_PORT &= ~(1 << LCD_D5_PIN);
	}
	
	if (nibble & (1 << 2))
	{
		LCD_D6_PORT |= (1 << LCD_D6_PIN);
	}
	else
	{
		LCD_D6_PORT &= ~(1 << LCD_D6_PIN);
	}
	
	if (nibble & (1 << 3))
	{
		LCD_D7_PORT |= (1 << LCD_D7_PIN);
	}
	else
	{
		LCD_D7_PORT &= ~(1 << LCD_D7_PIN);
	}
}

static void LCD_PulseEnable(void)
{
	// ¬ даташите: Timing Characteristics: минимальна€ ширина импульса E Ч 230 нс, минимальный полный цикл E Ч 500 нс
	// 1 мкс = 1000 нс
	LCD_E_PORT |= (1 << LCD_E_PIN);
	_delay_us(1);

	LCD_E_PORT &= ~(1 << LCD_E_PIN);
	_delay_us(1);
}

static void LCD_WriteByte(uint8_t data, bool isData)
{
	if (isData)
	{
		LCD_RS_PORT |= (1 << LCD_RS_PIN);
	}
	else
	{
		LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
	}
	uint8_t highNibble = data >> 4;
	LCD_WriteNibble(highNibble);
	LCD_PulseEnable();
	
	uint8_t lowNibble = data & 0b00001111;
	LCD_WriteNibble(lowNibble);
	LCD_PulseEnable();
}

static void LCD_WriteCommand(uint8_t command)
{
	LCD_WriteByte(command, false);
	_delay_us(50);
}

static void LCD_WriteData(uint8_t data)
{
	LCD_WriteByte(data, true);
	_delay_us(50);
}

static void LCD_WriteCommandNibble(uint8_t nibble)
{
	LCD_RS_PORT &= ~(1 << LCD_RS_PIN); // RS = 0, команда
	LCD_WriteNibble(nibble);
	LCD_PulseEnable();
	_delay_us(50);
}

void LCD_Clear(void)
{
	LCD_WriteCommand(0x01);
	_delay_ms(2);
}

void LCD_Init(void)
{
	LCD_RS_DDR |= (1 << LCD_RS_PIN);
	LCD_E_DDR  |= (1 << LCD_E_PIN);
	LCD_D4_DDR |= (1 << LCD_D4_PIN);
	LCD_D5_DDR |= (1 << LCD_D5_PIN);
	LCD_D6_DDR |= (1 << LCD_D6_PIN);
	LCD_D7_DDR |= (1 << LCD_D7_PIN);
	
	LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
	LCD_E_PORT  &= ~(1 << LCD_E_PIN);
	
	_delay_ms(20);

	LCD_WriteCommandNibble(0x03);
	_delay_ms(5);

	LCD_WriteCommandNibble(0x03);
	_delay_us(150);

	LCD_WriteCommandNibble(0x03);
	_delay_us(150);

	LCD_WriteCommandNibble(0x02); // переход в 4-bit mode
	_delay_us(150);
	
	LCD_WriteCommand(0b00101000); // 4-bit, 2 lines, 5x8 font
	LCD_WriteCommand(0b00001100); // display on, cursor off, blink off
	LCD_WriteCommand(0b00000110); // cursor moves right
	LCD_Clear();            // clear display
}

void LCD_PrintChar(char c)
{
	LCD_WriteData(c);
}

void LCD_Print(const char *text)
{
	while(*text != '\0')
	{
		LCD_PrintChar(*text);
		text++;
	}
}
