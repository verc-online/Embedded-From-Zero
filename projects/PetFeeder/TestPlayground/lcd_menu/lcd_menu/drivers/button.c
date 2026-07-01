/*
 * button.c
 *
 * Created: 26.06.2026 12:38:17
 *  Author: HP
 */ 

#include "../config/config.h"
#include "button.h"
#include <stdbool.h>

static bool ButtonOk_IsPressed(void)
{
	return (BUTTON_OK_PINREG & (1 << BUTTON_OK_PIN)) == 0;
}

static bool ButtonUp_IsPressed(void)
{
	return (BUTTON_UP_PINREG & (1 << BUTTON_UP_PIN)) == 0;
}

static bool ButtonDown_IsPressed(void)
{
	return (BUTTON_DOWN_PINREG & (1 << BUTTON_DOWN_PIN)) == 0;
}

void ButtonOkUpDown_Init(void)
{
	BUTTON_OK_DDR &= ~(1 << BUTTON_OK_PIN);
	BUTTON_OK_PORT |= (1 << BUTTON_OK_PIN);

	BUTTON_UP_DDR &= ~(1 << BUTTON_UP_PIN);
	BUTTON_UP_PORT |= (1 << BUTTON_UP_PIN);

	BUTTON_DOWN_DDR &= ~(1 << BUTTON_DOWN_PIN);
	BUTTON_DOWN_PORT |= (1 << BUTTON_DOWN_PIN);
}

ButtonEvent Button_GetEvent(void)
{
	static bool okWasPressed = false;
	static bool upWasPressed = false;
	static bool downWasPressed = false;

	bool okIsPressed = ButtonOk_IsPressed();
	bool upIsPressed = ButtonUp_IsPressed();
	bool downIsPressed = ButtonDown_IsPressed();

	if (okIsPressed && !okWasPressed)
	{
		okWasPressed = true;
		return BUTTON_EVENT_OK;
	}

	if (upIsPressed && !upWasPressed)
	{
		upWasPressed = true;
		return BUTTON_EVENT_UP;
	}

	if (downIsPressed && !downWasPressed)
	{
		downWasPressed = true;
		return BUTTON_EVENT_DOWN;
	}

	if (!okIsPressed)
	{
		okWasPressed = false;
	}

	if (!upIsPressed)
	{
		upWasPressed = false;
	}

	if (!downIsPressed)
	{
		downWasPressed = false;
	}

	return BUTTON_EVENT_NONE;
}