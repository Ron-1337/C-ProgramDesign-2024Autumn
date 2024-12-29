#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int show_menu(char *title, MenuList menu_list[], int count) {
  // 菜单项
  int currentChoice = 0;
  char key;

  while (1) {
    system("cls");
    printf("\n========%s========\n", title);

    // 显示菜单项
    for (int i = 0; i < count; i++) {
      if (i == currentChoice) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
        printf("[%c] %s", 'A' + i, menu_list[i].name);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
        printf("\n");
      } else {
        printf("[%c] %s\n", 'A' + i, menu_list[i].name);
      }
    }
    printf("\n");
    printf("使用方向键↑↓选择, 回车确认, 或直接按选项字母\n");

    // 读取按键
    key = _getch();
    if (_kbhit()) {
      key = _getch();
      switch (key) {
        case KEY_UP:
          currentChoice = (currentChoice - 1 + count) % count;
          break;
        case KEY_DOWN:
          currentChoice = (currentChoice + 1) % count;
          break;
      }
    } else if (key == KEY_ENTER) {
      return menu_list[currentChoice].value;
    } else if (toupper(key) >= 'A' && toupper(key) <= 'A' + count - 1) {
      currentChoice = toupper(key) - 'A';
      return menu_list[currentChoice].value;
    }
    continue;
  }
  return 0;
}

Date get_today() {
  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  return (Date){
      .year = tm->tm_year + 1900, .month = tm->tm_mon + 1, .day = tm->tm_mday};
}

Date get_next_day(Date date) {
  Date next_day = date;
  next_day.day++;

  int is_leap_year =
      (date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0);

  int max_days = days_in_month[next_day.month - 1];
  if (next_day.month == 2 && is_leap_year) {
    max_days++;
  }

  if (next_day.day > max_days) {
    next_day.day = 1;
    next_day.month++;
    if (next_day.month > 12) {
      next_day.month = 1;
      next_day.year++;
    }
  }

  return next_day;
}
int is_today(Date date) {
  Date today = get_today();
  return date.year == today.year && date.month == today.month &&
         date.day == today.day;
}

int is_tomorrow(Date date) {
  Date today = get_today();

  if (date.month < 1 || date.month > 12 || date.day < 1 || date.year < 0) {
    return 0;
  }

  Date tomorrow = get_next_day(today);

  return (date.year == tomorrow.year && date.month == tomorrow.month &&
          date.day == tomorrow.day);
}

/*
请求示例
https://geoapi.qweather.com/v2/city/lookup?location=巴黎&key=
*/
int get_locationID(char *city_name) { return 0; }

/*
请求示例
https://devapi.qweather.com/v7/weather/3d?location=7FA1&key=
*/
int get_weather(char *weather, Date date, char *destination_name) { return 0; }

int get_html(char *url, char *response) { return 0; }