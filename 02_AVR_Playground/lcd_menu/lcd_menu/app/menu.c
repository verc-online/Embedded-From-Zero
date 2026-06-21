/*
* MENU.c
*
* Created: 20.06.2026 14:07:48
*  Author: HP
*/

#include "../drivers/lcd1602.h"

typedef enum
{
	MENU_STATE_MAIN_SCREEN,
	MENU_STATE_MENU,
	MENU_STATE_SHOW_SCHEDULE,
	MENU_STATE_ADD_FEEDING,
	MENU_STATE_DELETE_FEEDING,
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

typedef enum
{
	BUTTON_EVENT_NONE,
	BUTTON_EVENT_UP,
	BUTTON_EVENT_DOWN,
	BUTTON_EVENT_OK
} ButtonEvent;

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

#define MENU_ITEMS_COUNT (sizeof(menuItems) / sizeof(menuItems[0]))

static void Menu_RenderMainScreen(void)
{
	LCD_Clear();
	
	LCD_SetCursor(0, 0);
	LCD_Print("Kormushka v0.3");

	LCD_SetCursor(1, 0);
	LCD_Print("OK: Menu");
}

static void Menu_RenderMenu(void)
{
	LCD_Clear();
	
	LCD_SetCursor(0, 0);
	LCD_Print(" > ");
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
		menuState = MENU_STATE_ADD_FEEDING;
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
	LCD_SetCursor(0, 0);
	LCD_Print("Schedule");
	LCD_SetCursor(1, 0);
	LCD_Print("OK: Back");
}

static void Menu_RenderAddFeeding(void)
{
	LCD_Clear();
	LCD_SetCursor(0, 0);
	LCD_Print("Add Feeding");
	LCD_SetCursor(1, 0);
	LCD_Print("OK: Back");
}

static void Menu_RenderDeleteFeeding(void)
{
	LCD_Clear();
	LCD_SetCursor(0, 0);
	LCD_Print("Delete Feeding");
	LCD_SetCursor(1, 0);
	LCD_Print("OK: Back");
}

static void Menu_RenderSetTime(void)
{
	LCD_Clear();
	LCD_SetCursor(0, 0);
	LCD_Print("Set Time");
	LCD_SetCursor(1, 0);
	LCD_Print("OK: Back");
}

static ButtonEvent Menu_ReadButtonEvent(void)
{
	return BUTTON_EVENT_NONE;
}

void Menu_Init(void)
{
	menuState = MENU_STATE_MAIN_SCREEN;
	selectedItem = MENU_ITEM_SHOW_SCHEDULE;
	Menu_RenderMainScreen();
}

void Menu_Process(void)
{
	ButtonEvent event = Menu_ReadButtonEvent();

	if (event == BUTTON_EVENT_NONE)
	{
		return;
	}

	switch (menuState)
	{
		case MENU_STATE_MAIN_SCREEN:
		break;

		case MENU_STATE_MENU:
		break;

		case MENU_STATE_SHOW_SCHEDULE:
		break;

		case MENU_STATE_ADD_FEEDING:
		break;

		case MENU_STATE_DELETE_FEEDING:
		break;

		case MENU_STATE_SET_TIME:
		break;
	}
}