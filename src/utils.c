#include "utils.h"

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