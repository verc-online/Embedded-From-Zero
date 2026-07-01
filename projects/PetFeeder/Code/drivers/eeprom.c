/*
 * eeprom.c
 *
 * Created: 15.06.2026 11:54:45
 *  Author: HP
 */ 
#include <avr/io.h>
#include <stdint.h>
#include "eeprom.h"


uint8_t EEPROM_ReadByte(uint16_t address)
{
	// Ждём, пока закончится предыдущая запись EEPROM.
	// Пока EEWE = 1, EEPROM занята записью.
	while (EECR & (1 << EEWE))
	{
	}

	// Указываем адрес ячейки EEPROM, которую хотим прочитать.
	EEAR = address;

	// Запускаем чтение EEPROM.
	// После установки EERE данные из EEPROM[address]
	// попадут в регистр EEDR.
	EECR |= (1 << EERE);

	// Возвращаем прочитанный байт.
	return EEDR;
}

void EEPROM_WriteByte(uint16_t address, uint8_t data)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE))
	;
	/* Set up address and data registers */
	EEAR = address;
	EEDR = data;
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
}

void EEPROM_UpdateByte(uint16_t address, uint8_t data)
{
	if (EEPROM_ReadByte(address) == data)
	{
		return;
	}
	EEPROM_WriteByte(address, data);
}
