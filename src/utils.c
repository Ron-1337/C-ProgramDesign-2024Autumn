#include "utils.h"

int show_menu(char *title, MenuList menu_list[], int count) {
  // �˵���
  int currentChoice = 0;
  char key;

  while (1) {
    system("cls");
    printf("\n========%s========\n", title);

    // ��ʾ�˵���
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
    printf("ʹ�÷��������ѡ��, �س�ȷ��, ��ֱ�Ӱ�ѡ����ĸ\n");

    // ��ȡ����
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