#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "define.h"

int show_menu(char *title, MenuList menu_list[], int count);

Date get_today();

int is_today(Date date);

int is_tomorrow(Date date);

int get_weather(char *weather, Date date, char *destination_name);
