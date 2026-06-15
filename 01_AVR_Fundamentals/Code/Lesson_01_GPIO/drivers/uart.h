/*
 * uart.h
 *
 * Created: 13.06.2026 9:36:14
 *  Author: HP
 */ 

#include <stdint.h>

#ifndef UART_H_
#define UART_H_

void UART_Init(void);
void UART_SendChar(char c);
void UART_SendString(const char *srt);
void UART_SendNumber(uint16_t value);
void UART_SendLine(const char *str);
void UART_SendNumber2(uint8_t value);
void UART_SendTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

#endif /* UART_H_ */