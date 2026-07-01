/*
 * button.h
 *
 * Created: 26.06.2026 12:38:25
 *  Author: HP
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_


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