/*
* MENU.c
*
* Created: 20.06.2026 14:07:48
*  Author: HP
*/
#include "../config/config.h"
#include "../drivers/lcd1602.h"
#include "../drivers/button.h"

#include <stdbool.h>

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




void Menu_Init(void)
{
	ButtonOkUpDown_Init();
	menuState = MENU_STATE_MAIN_SCREEN;
	selectedItem = MENU_ITEM_SHOW_SCHEDULE;
	Menu_RenderMainScreen();
}

void Menu_Process(void)
{
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

						case MENU_STATE_ADD_FEEDING:
						Menu_RenderAddFeeding();
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
		case MENU_STATE_ADD_FEEDING:
		case MENU_STATE_DELETE_FEEDING:
		case MENU_STATE_SET_TIME:
		if (event == BUTTON_EVENT_OK)
		{
			menuState = MENU_STATE_MENU;
			Menu_RenderMenu();
		}
		break;
	}
}