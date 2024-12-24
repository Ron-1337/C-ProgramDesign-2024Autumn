#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13

/* ���� */
typedef struct Date {
  int year;
  int month;
  int day;
} Date;

Date today;

/* �û��ȼ� */
typedef enum Level { BRONZE = 1, SILVER, GOLD, DIAMOND } Level;

/* ������Ϣ */
typedef struct Order {
  char name[20];
  int level;
  char destination[20];
  Date startDate;
  int days;
  double amount;
  struct Order* next;
} Order;

/* �˵��ṹ�� */
typedef struct Menu {
  char* items;
  int (*callback_function)();
} Menu;

/* �˵� */
int init();
int menu();
int create_order();
int exit_program();

// ���ȫ�ֱ��������涩�������ͷָ��
Order* head = NULL;

int main() {
  int flag = 0;
  while (flag != -1) {
    flag = menu();
  }
  return 0;
}

int init() {
  /* ��ǰ���� */
  time_t now = time(NULL);
  struct tm* tm = localtime(&now);
  today.year = tm->tm_year + 1900;
  today.month = tm->tm_mon + 1;
  today.day = tm->tm_mday;
  return 0;
}

int menu() {
  // �˵���
  const Menu menu[] = {{"��������", create_order}, {"�˳�����", exit_program}};
  const int MENU_ITEMS = sizeof(menu) / sizeof(menu[0]);
  int currentChoice = 0;
  char key;

  while (1) {
    system("cls");
    printf("\n=====������Ϣ����ϵͳ=====\n");
    printf("��ǰ���ڣ�%d-%d-%d\n", today.year, today.month, today.day);

    // ��ʾ�˵���
    for (int i = 0; i < MENU_ITEMS; i++) {
      if (i == currentChoice) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
        printf("[%c] %s", 'A' + i, menu[i].items);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
        printf("\n");
      } else {
        printf("[%c] %s\n", 'A' + i, menu[i].items);
      }
    }
    printf("============================================\n");
    printf("ʹ�÷��������ѡ�񣬻س�ȷ�ϣ���ֱ�Ӱ�ѡ����ĸ\n");

    // ��ȡ����
    key = _getch();
    if (_kbhit()) {
      key = _getch();
      switch (key) {
        case KEY_UP:
          currentChoice = (currentChoice - 1 + MENU_ITEMS) % MENU_ITEMS;
          break;
        case KEY_DOWN:
          currentChoice = (currentChoice + 1) % MENU_ITEMS;
          break;
      }
    } else if (key == KEY_ENTER) {
      return menu[currentChoice].callback_function();
    } else if (toupper(key) >= 'A' && toupper(key) <= 'A' + MENU_ITEMS - 1) {
      currentChoice = toupper(key) - 'A';
      return menu[currentChoice].callback_function();
    }
    continue;
  }
  return 0;
}

int create_order() {
  Order* new_order = (Order*)malloc(sizeof(Order));
  if (new_order == NULL) {
    printf("�ڴ����ʧ�ܣ�\n");
    return 0;
  }

  system("cls");
  printf("\n=====�����¶���=====\n");

  printf("������������");
  scanf("%s", new_order->name);

  printf("��ѡ���û��ȼ���1-ͭ�ƣ�2-���ƣ�3-���ƣ�4-��ʯ����");
  scanf("%d", &new_order->level);

  printf("������Ŀ�ĵأ�");
  scanf("%s", new_order->destination);

  printf("������������ڣ��� �� �գ���");
  scanf("%d %d %d", &new_order->startDate.year, &new_order->startDate.month,
        &new_order->startDate.day);

  printf("����������������");
  scanf("%d", &new_order->days);

  printf("�����붩����");
  scanf("%lf", &new_order->amount);

  // ���½ڵ���뵽����ͷ��
  new_order->next = head;
  head = new_order;

  printf("\n���������ɹ���\n");
  system("pause");
  return 0;
}

int exit_program() {
  // �ͷ������ڴ�
  Order* current = head;
  while (current != NULL) {
    Order* temp = current;
    current = current->next;
    free(temp);
  }

  printf("�����˳�\n");
  return -1;
}
