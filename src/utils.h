#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

#include "define.h"

extern const char* API_KEY;

int show_menu(char* title, MenuList menu_list[], int count);

Date get_today();

Date get_next_day(Date date);

int time_diff(Date date);

int get_locationID(char* city_name);

// HTTP请求函数
char* http_get(const wchar_t* server, const wchar_t* path, size_t* resultSize);

// URL编码函数
char* url_encode(const char* str);