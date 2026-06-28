/*
* MENU.c
*
* Created: 20.06.2026 14:07:48
*  Author: HP
*/
#include "../config/config.h"
#include "../drivers/lcd1602.h"
#include "../drivers/button.h"
#include "../app/scheduler.h"
#include "../drivers/ds3231.h"
#include "../drivers/timer.h"
#include "../app/scheduler.h"

#include <stdbool.h>

typedef enum
{
	MENU_STATE_MAIN_SCREEN,
	MENU_STATE_MENU,
	MENU_STATE_SHOW_SCHEDULE,
	MENU_STATE_ADD_FEEDING_HOURS,
	MENU_STATE_ADD_FEEDING_MINUTES,
	MENU_STATE_ADD_FEEDING_RESULT,
	MENU_STATE_DELETE_FEEDING,
	MENU_STATE_DELETE_FEEDING_RESULT,
	MENU_STATE_SET_TIME
	
} MenuState;

typedef enum
{
	MENU_ITEM_SHOW_SCHEDULE,
	MENU_ITEM_ADD_FEEDING,
	MENU_ITEM_DELETE_FEEDING,
	MENU_ITEM_SET_TIME,
	MENU_ITEM_BACK
} MenuItem;

static MenuState menuState = MENU_STATE_MAIN_SCREEN;
static MenuItem selectedItem = MENU_ITEM_SHOW_SCHEDULE;

static const char *menuItems[] =
{
	"Show Schedule",
	"Add Feeding",
	"Delete Feeding",
	"Set Time",
	"Back"
};
static uint8_t scheduleFirstVisibleIndex = 0;
static uint8_t editHours = 8;
static uint8_t editMinutes = 0;
static bool lastOperationSuccess = false;	
static uint8_t deleteSelectedIndex = 0;
	
#define MENU_ITEMS_COUNT (sizeof(menuItems) / sizeof(menuItems[0]))

static void Menu_RenderMainScreen(void)
{
	LCD_Clear();

	RtcTime time;

	if (DS3231_ReadTime(&time))
	{
		LCD_SetCursor(0, 0);
		LCD_Print2Digits(time.hours);
		LCD_Print(":");
		LCD_Print2Digits(time.minutes);
		LCD_Print(":");
		LCD_Print2Digits(time.seconds);
	}
	else
	{
		LCD_SetCursor(0, 0);
		LCD_Print("RTC Error");
	}

	LCD_SetCursor(1, 0);
	LCD_Print("OK: Menu");
}

static void Menu_RenderMenu(void)
{
	LCD_Clear();
	
	LCD_SetCursor(0, 0);
	LCD_Print("> ");
	LCD_Print(menuItems[selectedItem]);
	
	LCD_SetCursor(1, 0);
	LCD_Print("OK - select");
}

static void Menu_SelectCurrentItem(void)
{
	switch (selectedItem)
	{
		case MENU_ITEM_SHOW_SCHEDULE:
		menuState = MENU_STATE_SHOW_SCHEDULE;
		break;

		case MENU_ITEM_ADD_FEEDING:
		menuState = MENU_STATE_ADD_FEEDING_HOURS;
		break;

		case MENU_ITEM_DELETE_FEEDING:
		menuState = MENU_STATE_DELETE_FEEDING;
		break;

		case MENU_ITEM_SET_TIME:
		menuState = MENU_STATE_SET_TIME;
		break;

		case MENU_ITEM_BACK:
		menuState = MENU_STATE_MAIN_SCREEN;
		break;
	}
}

static void Menu_RenderShowSchedule(void)
{
	LCD_Clear();

	FeedingTime time;

	LCD_SetCursor(0, 0);
	if (Scheduler_GetFeedingTime(scheduleFirstVisibleIndex, &time))
	{
		LCD_Print2Digits(time.hours);
		LCD_Print(":");
		LCD_Print2Digits(time.minutes);
	}
	else
	{
		LCD_Print("Schedule is empty");
		LCD_SetCursor(1, 0);
		LCD_Print("Add feeding time");
		return;
	}

	LCD_SetCursor(1, 0);
	if (Scheduler_GetFeedingTime(scheduleFirstVisibleIndex + 1, &time))
	{
		LCD_Print2Digits(time.hours);
		LCD_Print(":");
		LCD_Print2Digits(time.minutes);
	}
	else
	{
		LCD_Print("OK: Back");
	}
}

static void Menu_RenderAddFeedingHours(void)
{
	LCD_Clear();
	LCD_SetCursor(0, 0);
	LCD_Print("Hours/OK->Minutes");
	LCD_SetCursor(1, 0);
	LCD_Print2Digits(editHours);
}

static void Menu_RenderAddFeedingMinutes(void)
{
	LCD_Clear();
	LCD_SetCursor(0, 0);
	LCD_Print("Minutes/OK->Save");
	LCD_SetCursor(1, 0);
	LCD_Print2Digits(editMinutes);
}

static void Menu_RenderAddFeedingResult(void)
{
	const char *text = lastOperationSuccess ? "Added" : "Error";
	LCD_Clear();
	LCD_SetCursor(0, 0);
	LCD_Print(text);
	LCD_SetCursor(1, 0);
	LCD_Print2Digits(editHours);
	LCD_Print(":");
	LCD_Print2Digits(editMinutes);
}

static void Menu_RenderDeleteFeeding(void)
{
	LCD_Clear();
	LCD_SetCursor(0, 0);
	FeedingTime time;
	// Проверяем, если расписание пустое
	if (Scheduler_GetFeedingCount() == 0)
	{
		LCD_Print("Nothing to delete");
		LCD_SetCursor(1, 0);
		LCD_Print("OK: Back");
		return;
	}
	if (Scheduler_GetFeedingTime(deleteSelectedIndex, &time))
	{
		LCD_Print("Delete:");
		LCD_SetCursor(1, 0);
		LCD_Print("> ");
		LCD_Print2Digits(time.hours);
		LCD_Print(":");
		LCD_Print2Digits(time.minutes);
	}
}

static void Menu_RenderDeleteFeedingResult(void)
{
	const char *text = lastOperationSuccess ? "Deleted" : "Error";
	LCD_Clear();
	LCD_SetCursor(0, 0);
	LCD_Print(text);
	
	if (Scheduler_GetFeedingCount() == 0)
	{
		LCD_SetCursor(1, 0);
		LCD_Print("Schedule is empty");
	}
}

static void Menu_RenderSetTime(void)
{
	LCD_Clear();
	LCD_SetCursor(0, 0);
	LCD_Print("Set Time");
	LCD_SetCursor(1, 0);
	LCD_Print("OK: Back");
}

void Menu_Init(void)
{
	ButtonOkUpDown_Init();
	menuState = MENU_STATE_MAIN_SCREEN;
	selectedItem = MENU_ITEM_SHOW_SCHEDULE;
	Menu_RenderMainScreen();
}

void Menu_Process(void)
{
	static uint32_t lastRefreshTime;
	if (menuState == MENU_STATE_MAIN_SCREEN)
	{
		if (Timer_HasElapsed(lastRefreshTime, MENU_MAIN_REFRESH_TICKS))
		{
			lastRefreshTime = Timer_GetTicks();
			Menu_RenderMainScreen();
		}
	}
	
	ButtonEvent event = Button_GetEvent();

	if (event == BUTTON_EVENT_NONE)
	{
		return;
	}

	switch (menuState)
	{
		case MENU_STATE_MAIN_SCREEN:
			if (event == BUTTON_EVENT_OK)
			{
				menuState = MENU_STATE_MENU;
				Menu_RenderMenu();
			}
			break;

		case MENU_STATE_MENU:
			switch(event)
			{
				case BUTTON_EVENT_UP:
					if (selectedItem > MENU_ITEM_SHOW_SCHEDULE)
					{
						selectedItem--;
						Menu_RenderMenu();
					}
					break;

				case BUTTON_EVENT_DOWN:
					if (selectedItem < MENU_ITEM_BACK)
					{
						selectedItem++;
						Menu_RenderMenu();
					}
					break;

				case BUTTON_EVENT_OK:
					Menu_SelectCurrentItem();

					switch (menuState)
					{
						case MENU_STATE_MAIN_SCREEN:
						Menu_RenderMainScreen();
						break;

						case MENU_STATE_SHOW_SCHEDULE:
						Menu_RenderShowSchedule();
						break;
						
						case MENU_STATE_ADD_FEEDING_HOURS:
						Menu_RenderAddFeedingHours();
						break;

						case MENU_STATE_DELETE_FEEDING:
						Menu_RenderDeleteFeeding();
						break;

						case MENU_STATE_SET_TIME:
						Menu_RenderSetTime();
						break;

						default:
						break;
					}
					break;
				
				default:
					break;
			}
			break;

		case MENU_STATE_SHOW_SCHEDULE:
			switch(event)
			{
				case BUTTON_EVENT_OK:
					menuState = MENU_STATE_MENU;
					Menu_RenderMenu();
					break;
					
				case BUTTON_EVENT_UP:
					if (scheduleFirstVisibleIndex > 0)
					{
						scheduleFirstVisibleIndex--;
						Menu_RenderShowSchedule();
					}
					break;
					
				case BUTTON_EVENT_DOWN:
					if ((scheduleFirstVisibleIndex + 1) < Scheduler_GetFeedingCount())
					{
						scheduleFirstVisibleIndex++;
						Menu_RenderShowSchedule();
					}
					break;
					
				case BUTTON_EVENT_NONE:
				default:
					break;
			}
			break;
			
		case MENU_STATE_ADD_FEEDING_HOURS:
			switch(event)
			{
				case BUTTON_EVENT_OK:
					menuState = MENU_STATE_ADD_FEEDING_MINUTES;
					Menu_RenderAddFeedingMinutes();
					break;

				case BUTTON_EVENT_UP:
					if (editHours >= 23) editHours = 0;
					else editHours++;
					Menu_RenderAddFeedingHours();
					break;
					
				case BUTTON_EVENT_DOWN:
					if (editHours == 0) editHours = 23;
					else editHours--;
					Menu_RenderAddFeedingHours();
					break;
					
				case BUTTON_EVENT_NONE:
				default:
					break;
			}
			break;
		case MENU_STATE_ADD_FEEDING_MINUTES:
			switch(event)
			{
				case BUTTON_EVENT_OK:
				lastOperationSuccess = Scheduler_AddFeedingTime(editHours, editMinutes);
				menuState = MENU_STATE_ADD_FEEDING_RESULT;
				Menu_RenderAddFeedingResult();
				break;
					
				case BUTTON_EVENT_UP:
					if (editMinutes == 59)
					{
						editMinutes = 0;
					} else editMinutes++;
					Menu_RenderAddFeedingMinutes();
					break;
					
				case BUTTON_EVENT_DOWN:
					if (editMinutes == 0) editMinutes = 59;
					else editMinutes--;
					Menu_RenderAddFeedingMinutes();
					break;
					
				case BUTTON_EVENT_NONE:
				default:
					break;
			}
			break;
		case  MENU_STATE_ADD_FEEDING_RESULT:
			switch(event)
			{
				case BUTTON_EVENT_OK:
				case BUTTON_EVENT_UP:
				case BUTTON_EVENT_DOWN:
				menuState = MENU_STATE_SHOW_SCHEDULE;
				Menu_RenderShowSchedule();
				break;
				
				case BUTTON_EVENT_NONE:
				default: break;
			}
			break;
		case MENU_STATE_DELETE_FEEDING:
			switch(event)
			{
				case BUTTON_EVENT_OK:
				if (Scheduler_GetFeedingCount() == 0)
				{
					menuState = MENU_STATE_SHOW_SCHEDULE;
					Menu_RenderShowSchedule();
					break;
				}
				lastOperationSuccess = Scheduler_DeleteFeedingTime(deleteSelectedIndex);
				
				uint8_t count = Scheduler_GetFeedingCount();

				if (count == 0)
				{
					deleteSelectedIndex = 0;
				}
				else if (deleteSelectedIndex >= count)
				{
					deleteSelectedIndex = count - 1;
				}
				
				menuState = MENU_STATE_DELETE_FEEDING_RESULT;
				Menu_RenderDeleteFeedingResult();
				break;
				
				case BUTTON_EVENT_UP:
					if (deleteSelectedIndex == 0) break;
					deleteSelectedIndex--;
					Menu_RenderDeleteFeeding();
					break;
				
				case BUTTON_EVENT_DOWN:
					if ((deleteSelectedIndex + 1) < Scheduler_GetFeedingCount())
					{
						deleteSelectedIndex++;
						Menu_RenderDeleteFeeding();
					}
					break;
				
				case BUTTON_EVENT_NONE:
				default: break;
			}
			break;
		case MENU_STATE_DELETE_FEEDING_RESULT:
			switch(event)
			{
				case BUTTON_EVENT_OK:
				case BUTTON_EVENT_UP:
				case BUTTON_EVENT_DOWN:
				menuState = MENU_STATE_SHOW_SCHEDULE;
				Menu_RenderShowSchedule();
				break;
				
				case BUTTON_EVENT_NONE:
				default: break;
			}
			break;
		case MENU_STATE_SET_TIME:
		if (event == BUTTON_EVENT_OK)
		{
			menuState = MENU_STATE_MENU;
			Menu_RenderMenu();
		}
		break;
	}
}