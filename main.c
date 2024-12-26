#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "utils.h"

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
const char* LEVEL_NAME[] = {"青铜", "白银", "黄金", "钻石"};

/* 订单信息 */
typedef struct Order {
  char name[8];
  int level;
  char destination[10];
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

int init();
int show_menu(const Menu menu_list[], int menu_size);
int create_order();
int show_order();
int search_order();
int save_order();
int load_order();
int release_memory();
int exit_program();

// 添加全局变量来保存订单链表的头指针
Order* head = NULL;

int main() {
  init();
  int flag = 0;
  const Menu menu_list[] = {
      {"创建订单", create_order}, {"显示订单", show_order},
      {"搜索订单", search_order}, {"保存订单", save_order},
      {"加载订单", load_order},   {"退出程序", exit_program},
  };
  while (flag != -1) {
    flag = show_menu(menu_list, sizeof(menu_list) / sizeof(menu_list[0]));
  }
  return 0;
}

int init() {
  // 加载订单
  load_order();

  /* 当前日期 */
  time_t now = time(NULL);
  struct tm* tm = localtime(&now);
  today.year = tm->tm_year + 1900;
  today.month = tm->tm_mon + 1;
  today.day = tm->tm_mday;
  return 0;
}

int show_menu(const Menu menu_list[], int menu_size) {
  // 菜单项
  int currentChoice = 0;
  char key;

  while (1) {
    system("cls");
    printf("\n=====旅游信息管理系统=====\n");
    printf("当前日期：%d-%d-%d\n", today.year, today.month, today.day);

    // 显示菜单项
    for (int i = 0; i < menu_size; i++) {
      if (i == currentChoice) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
        printf("[%c] %s", 'A' + i, menu_list[i].items);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
        printf("\n");
      } else {
        printf("[%c] %s\n", 'A' + i, menu_list[i].items);
      }
    }
    printf("==========================\n");
    printf("使用方向键↑↓选择，回车确认，或直接按选项字母\n");

    // 读取按键
    key = _getch();
    if (_kbhit()) {
      key = _getch();
      switch (key) {
        case KEY_UP:
          currentChoice = (currentChoice - 1 + menu_size) % menu_size;
          break;
        case KEY_DOWN:
          currentChoice = (currentChoice + 1) % menu_size;
          break;
      }
    } else if (key == KEY_ENTER) {
      return menu_list[currentChoice].callback_function();
    } else if (toupper(key) >= 'A' && toupper(key) <= 'A' + menu_size - 1) {
      currentChoice = toupper(key) - 'A';
      return menu_list[currentChoice].callback_function();
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

  do {
    printf("请输入姓名：");
    scanf("%8s", new_order->name);
  } while (checkName(new_order->name));

  do {
    printf("请选择用户等级（1-铜牌，2-银牌，3-金牌，4-钻石）：");
    scanf("%d", &new_order->level);
  } while (checkLevel(new_order->level));

  do {
    printf("请输入目的地：");
    scanf("%10s", new_order->destination);
  } while (checkDestination(new_order->destination));

  do {
    printf("请输入出发日期（年 月 日）：");
    scanf("%d %d %d", &new_order->startDate.year, &new_order->startDate.month,
          &new_order->startDate.day);
  } while (checkDate(new_order->startDate.year, new_order->startDate.month,
                     new_order->startDate.day));

  do {
    printf("请输入旅游天数：");
    scanf("%d", &new_order->days);
  } while (checkDays(new_order->days));

  do {
    printf("请输入订单金额：");
    scanf("%lf", &new_order->amount);
  } while (checkAmount(new_order->amount));

  // 将新节点插入到链表头部
  new_order->next = head;
  head = new_order;

  printf("\n订单创建成功！\n");
  system("pause");
  return 0;
}

int exit_program() {
  // 释放链表内存
  release_memory();
  printf("程序退出\n");
  return -1;
}

int show_order() {
  system("cls");
  Order* current = head;
  printf("%8s %5s %10s  %10s %8s %8s\n", "姓名", "等级", "目的地", "出发日期",
         "旅游天数", "订单金额");
  while (current != NULL) {
    printf("%8s %5s %10s  %4d-%02d-%02d %8d %8.2lf\n", current->name,
           LEVEL_NAME[current->level - 1], current->destination,
           current->startDate.year, current->startDate.month,
           current->startDate.day, current->days, current->amount);
    current = current->next;
  }
  system("pause");
  return 0;
}

int search_order() {
  printf("搜索订单\n");
  return 0;
}

int save_order() {
  FILE* file = fopen("data.txt", "w");
  if (file == NULL) {
    printf("无法打开文件！\n");
    return 0;
  }

  Order* current = head;
  while (current != NULL) {
    fprintf(file, "%s %d %s %d-%d-%d %d %.2f\n", current->name, current->level,
            current->destination, current->startDate.year,
            current->startDate.month, current->startDate.day, current->days,
            current->amount);
    current = current->next;
  }

  fclose(file);
  printf("数据已保存到data.txt\n");
  return 0;
}

int load_order() {
  printf("正在从data.txt加载数据...\n");
  release_memory();
  FILE* file = fopen("data.txt", "r");
  if (file == NULL) {
    printf("无法打开文件！\n");
    system("pause");
    return 0;
  }

  int count = 0;
  while (!feof(file)) {
    Order* new_order = (Order*)malloc(sizeof(Order));
    if (new_order == NULL) {
      printf("内存分配失败！\n");
      break;
    }

    if (fscanf(file, "%s %d %s %d-%d-%d %d %lf", new_order->name,
               &new_order->level, new_order->destination,
               &new_order->startDate.year, &new_order->startDate.month,
               &new_order->startDate.day, &new_order->days,
               &new_order->amount) == 8) {
      // 将新节点插入到链表头部
      new_order->next = head;
      head = new_order;
      count++;
    } else {
      free(new_order);
      break;
    }
  }

  fclose(file);
  printf("OK\n");
  printf("加载了%d条数据\n", count);
  return 0;
}

int release_memory() {
  Order* current = head;
  while (current != NULL) {
    Order* temp = current;
    current = current->next;
    free(temp);
  }
  head = NULL;
  return 0;
}
