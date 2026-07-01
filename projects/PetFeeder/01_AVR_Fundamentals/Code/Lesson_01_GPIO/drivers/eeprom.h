/*
 * eeprom.h
 *
 * Created: 15.06.2026 11:54:36
 *  Author: HP
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

uint8_t EEPROM_ReadByte(uint16_t address);
void EEPROM_WriteByte(uint16_t address, uint8_t data);
void EEPROM_UpdateByte(uint16_t address, uint8_t data);

#endif /* EEPROM_H_ */