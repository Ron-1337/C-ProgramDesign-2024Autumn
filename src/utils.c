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

  int is_leap_year =
      (date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0);

  Date tomorrow = today;
  tomorrow.day++;

  int max_days = days_in_month[tomorrow.month - 1];
  if (tomorrow.month == 2 && is_leap_year) {
    max_days++;
  }

  if (tomorrow.day > max_days) {
    tomorrow.day = 1;
    tomorrow.month++;
    if (tomorrow.month > 12) {
      tomorrow.month = 1;
      tomorrow.year++;
    }
  }

  return (date.year == tomorrow.year && date.month == tomorrow.month &&
          date.day == tomorrow.day);
}

int get_weather(char *weather, Date date, char *destination_name) { return 0; }