/*
* structures.c
*
* Created: 19.06.2026 15:48:18
* Author : HP
*/

#include <avr/io.h>

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
} FeedingTime;

FeedingTime schedule[3] =
{
	{8, 0},
	{12, 0},
	{18, 0}
};

void SaveScheduleToBuffer(uint8_t *buffer,
						  const FeedingTime *schedule,
						  uint8_t count)
{
	for (uint8_t i = 0; i < count; i++)
	{
		const uint8_t *timeBytes = (const uint8_t*)&schedule[i];

		for (uint8_t k = 0; k < sizeof(FeedingTime); k++)
		{
			buffer[i * sizeof(FeedingTime) + k] = timeBytes[k];
		}
	}
}

typedef struct
{
	uint8_t a;
	uint16_t b;
	uint8_t c;
} Test;

int main(void)
{
	int value = 10;

	int *ptr = &value;

	volatile uint8_t sizePtr = sizeof(ptr);
	volatile uint8_t sizeInt = sizeof(value);
	uint8_t buffer[sizeof(schedule)];
	SaveScheduleToBuffer(buffer, schedule, 3);
	volatile uint16_t size = sizeof(Test);
	while (1)
	{
		size = sizeof(Test);
	}
}

