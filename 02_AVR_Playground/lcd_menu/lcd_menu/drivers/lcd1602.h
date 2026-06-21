/*
 * lcd1602.h
 *
 * Created: 20.06.2026 14:07:39
 *  Author: HP
 */ 


#ifndef LCD1602_H_
#define LCD1602_H_

#include <stdint.h>
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_PrintChar(char c);
void LCD_Print(const char *text);

#endif /* LCD1602_H_ */