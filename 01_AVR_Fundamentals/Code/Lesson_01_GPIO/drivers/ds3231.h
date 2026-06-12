/*
* ds3231.h
*
* Created: 12.06.2026 14:39:27
*  Author: HP
*/


#ifndef DS3231_H_
#define DS3231_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} RtcTime;


static uint8_t BcdToDecimal(uint8_t bcd);
static uint8_t DecimalToBcd(uint8_t value);

bool DS3231_ReadTime(RtcTime *time);
bool DS3231_SetTime(const RtcTime *time);

#endif /* DS3231_H_ */