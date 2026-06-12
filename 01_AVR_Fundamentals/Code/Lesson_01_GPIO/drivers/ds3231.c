/*
 * ds3231.c
 *
 * Created: 12.06.2026 14:39:21
 *  Author: HP
 */ 

#include "ds3231.h"
#include "twi.h"

#define DS3231_ADDRESS 0x68
#define DS3231_REG_SECONDS 0x00
#define DS3231_WRITE ((DS3231_ADDRESS << 1) | 0)
#define DS3231_READ  ((DS3231_ADDRESS << 1) | 1)

static uint8_t BcdToDecimal(uint8_t bcd)
{
	uint8_t tens = bcd >> 4;
	uint8_t ones = bcd & 0x0F;

	return tens * 10 + ones;
}

static uint8_t DecimalToBcd(uint8_t value)
{
	uint8_t tens = value / 10;
	uint8_t ones = value % 10;

	return (tens << 4) | ones;
}

bool DS3231_ReadTime(RtcTime *time)
{
	// Создаем START condition.
	// Сообщаем всем устройствам на шине, что начинается новая транзакция.
	if (!TWI_Start())
	{
		return false;
	}

	// Отправляем адрес DS3231 в режиме WRITE.
	//
	// Почему WRITE?
	// Потому что сначала нам нужно указать,
	// из какого регистра мы хотим читать данные.
	//
	// Ожидаем статус 0x18:
	// SLA+W transmitted, ACK received.
	//
	// Это означает:
	// - адрес отправлен
	// - DS3231 существует на шине
	// - DS3231 подтвердил прием адреса
	if (!TWI_Write(DS3231_WRITE, 0x18))
	{
		TWI_Stop();
		return false;
	}

	// Указываем первый регистр для чтения.
	//
	// Регистр 0x00 = Seconds.
	//
	// После этого внутренний указатель DS3231
	// будет установлен на начало блока времени.
	//
	// Ожидаем статус 0x28:
	// Data transmitted, ACK received.
	if (!TWI_Write(DS3231_REG_SECONDS, 0x28))
	{
		TWI_Stop();
		return false;
	}

	// Создаем Repeated START.
	//
	// Не отпуская шину, переключаемся
	// из режима записи в режим чтения.
	if (!TWI_Start())
	{
		TWI_Stop();
		return false;
	}

	// Отправляем адрес DS3231 в режиме READ.
	//
	// Теперь DS3231 понимает:
	// "Адрес регистра уже задан,
	// начинаем выдавать данные."
	//
	// Ожидаем статус 0x40:
	// SLA+R transmitted, ACK received.
	//
	// Это означает:
	// - адрес отправлен
	// - режим чтения принят
	// - DS3231 готов передавать данные
	if (!TWI_Write(DS3231_READ, 0x40))
	{
		TWI_Stop();
		return false;
	}

	// Читаем секунды.
	//
	// TWI_ReadAck():
	// - читаем байт
	// - отправляем ACK
	//
	// ACK означает:
	// "Байт получил, отправляй следующий."
	//
	// Значение приходит в BCD формате,
	// поэтому преобразуем его в обычное число.
	time->seconds = BcdToDecimal(TWI_ReadAck());

	// Читаем минуты.
	//
	// После чтения секунд DS3231 автоматически
	// сдвинул внутренний указатель на регистр минут.
	time->minutes = BcdToDecimal(TWI_ReadAck());

	// Читаем часы.
	//
	// Это последний нужный байт.
	//
	// Поэтому используем NACK:
	// "Байт получил, больше данных не нужно."
	time->hours = BcdToDecimal(TWI_ReadNack());

	// Освобождаем шину I2C.
	// Передача завершена.
	TWI_Stop();

	return true;

}

bool DS3231_SetTime(const RtcTime *time)
{
	if (!TWI_Start())
	{
		return false;
	}

	// Адрес DS3231 + WRITE
	if (!TWI_Write(DS3231_WRITE, 0x18))
	{
		TWI_Stop();
		return false;
	}

	// Указываем первый регистр: 0x00 = Seconds
	if (!TWI_Write(DS3231_REG_SECONDS, 0x28))
	{
		TWI_Stop();
		return false;
	}

	// Пишем seconds в регистр 0x00
	if (!TWI_Write(DecimalToBcd(time->seconds), 0x28))
	{
		TWI_Stop();
		return false;
	}

	// Пишем minutes в регистр 0x01
	if (!TWI_Write(DecimalToBcd(time->minutes), 0x28))
	{
		TWI_Stop();
		return false;
	}

	// Пишем hours в регистр 0x02
	if (!TWI_Write(DecimalToBcd(time->hours), 0x28))
	{
		TWI_Stop();
		return false;
	}

	TWI_Stop();
	return true;
}

