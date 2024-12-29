#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

#include "define.h"

extern const char *API_KEY;

int show_menu(char *title, MenuList menu_list[], int count);

Date get_today();

Date get_next_day(Date date);

int is_today(Date date);

int is_tomorrow(Date date);

int get_weather(char *weather, Date date, char *destination_name);

int get_html(char *url, char *response);

int get_locationID(char *city_name);
