/*
 * twi.h
 *
 * Created: 12.06.2026 12:33:55
 *  Author: HP
 */ 


#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>
#include <stdbool.h>

void TWI_Init();
bool TWI_Start(void);
bool TWI_Write(uint8_t data, uint8_t expectedStatus);
void TWI_Stop(void);

uint8_t TWI_ReadAck(void);
uint8_t TWI_ReadNack(void);


#endif /* TWI_H_ */