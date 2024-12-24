#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13

/* 日期 */
typedef struct Date {
  int year;
  int month;
  int day;
} Date;

Date today;

/* 用户等级 */
typedef enum Level { BRONZE = 1, SILVER, GOLD, DIAMOND } Level;

/* 订单信息 */
typedef struct Order {
  char name[20];
  int level;
  char destination[20];
  Date startDate;
  int days;
  double amount;
  struct Order* next;
} Order;

/* 菜单结构体 */
typedef struct Menu {
  char* items;
  int (*callback_function)();
} Menu;

/* 菜单 */
int init();
int menu();
int create_order();
int exit_program();

// 添加全局变量来保存订单链表的头指针
Order* head = NULL;

int main() {
  int flag = 0;
  while (flag != -1) {
    flag = menu();
  }
  return 0;
}

int init() {
  /* 当前日期 */
  time_t now = time(NULL);
  struct tm* tm = localtime(&now);
  today.year = tm->tm_year + 1900;
  today.month = tm->tm_mon + 1;
  today.day = tm->tm_mday;
  return 0;
}

int menu() {
  // 菜单项
  const Menu menu[] = {{"创建订单", create_order}, {"退出程序", exit_program}};
  const int MENU_ITEMS = sizeof(menu) / sizeof(menu[0]);
  int currentChoice = 0;
  char key;

  while (1) {
    system("cls");
    printf("\n=====旅游信息管理系统=====\n");
    printf("当前日期：%d-%d-%d\n", today.year, today.month, today.day);

    // 显示菜单项
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
    printf("使用方向键↑↓选择，回车确认，或直接按选项字母\n");

    // 读取按键
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
    printf("内存分配失败！\n");
    return 0;
  }

  system("cls");
  printf("\n=====创建新订单=====\n");

  printf("请输入姓名：");
  scanf("%s", new_order->name);

  printf("请选择用户等级（1-铜牌，2-银牌，3-金牌，4-钻石）：");
  scanf("%d", &new_order->level);

  printf("请输入目的地：");
  scanf("%s", new_order->destination);

  printf("请输入出发日期（年 月 日）：");
  scanf("%d %d %d", &new_order->startDate.year, &new_order->startDate.month,
        &new_order->startDate.day);

  printf("请输入旅游天数：");
  scanf("%d", &new_order->days);

  printf("请输入订单金额：");
  scanf("%lf", &new_order->amount);

  // 将新节点插入到链表头部
  new_order->next = head;
  head = new_order;

  printf("\n订单创建成功！\n");
  system("pause");
  return 0;
}

int exit_program() {
  // 释放链表内存
  Order* current = head;
  while (current != NULL) {
    Order* temp = current;
    current = current->next;
    free(temp);
  }

  printf("程序退出\n");
  return -1;
}
