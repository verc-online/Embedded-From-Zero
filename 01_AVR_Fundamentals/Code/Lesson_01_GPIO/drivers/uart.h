/*
 * uart.h
 *
 * Created: 13.06.2026 9:36:14
 *  Author: HP
 */ 


#ifndef UART_H_
#define UART_H_

void UART_Init(void);
void UART_SendChar(char c);
void UART_SendString(const char *srt);
void UART_SendNumber(uint16_t number);

#endif /* UART_H_ */