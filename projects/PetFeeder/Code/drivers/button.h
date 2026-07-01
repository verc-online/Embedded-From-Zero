/*
 * button.h
 *
 * Created: 10.06.2026 17:08:32
 *  Author: HP
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdbool.h>

// V2 Button driver
typedef enum
{
	BUTTON_EVENT_NONE,

	BUTTON_EVENT_OK,

	BUTTON_EVENT_UP,

	BUTTON_EVENT_DOWN

} ButtonEvent;

void ButtonOkUpDown_Init(void);

ButtonEvent Button_GetEvent(void);

#endif /* BUTTON_H_ */