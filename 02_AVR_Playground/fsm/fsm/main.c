/*
* fsm.c
*
* Created: 19.06.2026 19:00:41
* Author : HP
*/

#include <avr/io.h>

typedef enum
{
	LED_OFF,
	LED_ON
} LedState;

void Led_Process(LedState *state)
{
	static uint16_t ticks = 0;

	switch(*state)
	{
		case LED_OFF:

		if (ticks >= 1000)
		{
			*state = LED_ON;
		}

		break;

		case LED_ON:

		if (ticks >= 1000)
		{
			*state = LED_OFF;
		}

		break;
	}
	
	ticks++;
}

int main(void)
{

	LedState state = LED_OFF;
	
	while (1)
	{
		Led_Process(&state);
	}
}

