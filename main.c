#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "utils.h"

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
const char* LEVEL_NAME[] = {"��ͭ", "����", "�ƽ�", "��ʯ"};

/* ������Ϣ */
typedef struct Order {
  char name[8];
  int level;
  char destination[10];
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

int init();
int show_menu(const Menu menu_list[], int menu_size);
int create_order();
int show_order();
int search_order();
int save_order();
int load_order();
int release_memory();
int exit_program();

int search_by_name();
int search_by_level();
int search_by_destination();
int search_by_start_date();
int search_by_days();
int search_by_amount();

// ���ȫ�ֱ��������涩�������ͷָ��
Order* head = NULL;

int main() {
  init();
  int flag = 0;
  const Menu menu_list[] = {
      {"��������", create_order}, {"��ʾ����", show_order},
      {"��ѯ����", search_order}, {"���涩��", save_order},
      {"���ض���", load_order},   {"�˳�����", exit_program},
  };
  while (flag != -1) {
    flag = show_menu(menu_list, sizeof(menu_list) / sizeof(menu_list[0]));
  }
  return 0;
}

int init() {
  // ���ض���
  load_order();

  /* ��ǰ���� */
  time_t now = time(NULL);
  struct tm* tm = localtime(&now);
  today.year = tm->tm_year + 1900;
  today.month = tm->tm_mon + 1;
  today.day = tm->tm_mday;
  return 0;
}

int show_menu(const Menu menu_list[], int menu_size) {
  // �˵���
  int currentChoice = 0;
  char key;

  while (1) {
    system("cls");
    printf("\n=====������Ϣ����ϵͳ=====\n");
    printf("��ǰ���ڣ�%d-%d-%d\n", today.year, today.month, today.day);

    // ��ʾ�˵���
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
    printf("ʹ�÷��������ѡ�񣬻س�ȷ�ϣ���ֱ�Ӱ�ѡ����ĸ\n");

    // ��ȡ����
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
      if (menu_list[currentChoice].callback_function == NULL) {
        return 0;
      }
      return menu_list[currentChoice].callback_function();
    } else if (toupper(key) >= 'A' && toupper(key) <= 'A' + menu_size - 1) {
      currentChoice = toupper(key) - 'A';
      if (menu_list[currentChoice].callback_function == NULL) {
        return 0;
      }
      return menu_list[currentChoice].callback_function();
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

  do {
    printf("������������");
    scanf("%8s", new_order->name);
  } while (checkName(new_order->name));

  do {
    printf("��ѡ���û��ȼ���1-ͭ�ƣ�2-���ƣ�3-���ƣ�4-��ʯ����");
    scanf("%d", &new_order->level);
  } while (checkLevel(new_order->level));

  do {
    printf("������Ŀ�ĵأ�");
    scanf("%10s", new_order->destination);
  } while (checkDestination(new_order->destination));

  do {
    printf("������������ڣ��� �� �գ���");
    scanf("%d %d %d", &new_order->startDate.year, &new_order->startDate.month,
          &new_order->startDate.day);
  } while (checkDate(new_order->startDate.year, new_order->startDate.month,
                     new_order->startDate.day));

  do {
    printf("����������������");
    scanf("%d", &new_order->days);
  } while (checkDays(new_order->days));

  do {
    printf("�����붩����");
    scanf("%lf", &new_order->amount);
  } while (checkAmount(new_order->amount));

  // ���½ڵ���뵽����ͷ��
  new_order->next = head;
  head = new_order;

  printf("\n���������ɹ���\n");
  system("pause");
  return 0;
}

int exit_program() {
  // �ͷ������ڴ�
  release_memory();
  printf("�����˳�\n");
  return -1;
}

int show_order() {
  system("cls");
  Order* current = head;
  printf("%8s %5s %10s  %10s %8s %8s\n", "����", "�ȼ�", "Ŀ�ĵ�", "��������",
         "��������", "�������");
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
  const Menu search_menu[] = {
      {"��������ѯ", search_by_name},
      {"���ȼ���ѯ", search_by_level},
      {"��Ŀ�ĵز�ѯ", search_by_destination},
      {"���������ڲ�ѯ", search_by_start_date},
      {"������������ѯ", search_by_days},
      {"����������ѯ", search_by_amount},
      {"�������˵�", NULL},
  };

  show_menu(search_menu, sizeof(search_menu) / sizeof(search_menu[0]));
  return 0;
}

int search_by_name() {
  system("cls");
  printf("������Ҫ��ѯ��������");
  char name[8];
  scanf("%8s", name);
  Order* current = head;
  int hasResult = 0;
  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      hasResult = 1;
      printf("�ҵ�������%s %s %s %d-%d-%d %d %lf\n", current->name,
             LEVEL_NAME[current->level - 1], current->destination,
             current->startDate.year, current->startDate.month,
             current->startDate.day, current->days, current->amount);
    }
    current = current->next;
  }
  if (!hasResult) {
    printf("δ�ҵ�����\n");
  }
  system("pause");
  return 0;
}

int search_by_level() {
  system("cls");
  printf("������Ҫ��ѯ�ĵȼ���");
  int level;
  scanf("%d", &level);
  Order* current = head;
  int hasResult = 0;
  while (current != NULL) {
    if (current->level == level) {
      hasResult = 1;
      printf("�ҵ�������%s %s %s %d-%d-%d %d %lf\n", current->name,
             LEVEL_NAME[current->level - 1], current->destination,
             current->startDate.year, current->startDate.month,
             current->startDate.day, current->days, current->amount);
    }
    current = current->next;
  }
  if (!hasResult) {
    printf("δ�ҵ�����\n");
  }
  system("pause");
  return 0;
}

int search_by_destination() {
  system("cls");
  printf("������Ҫ��ѯ��Ŀ�ĵأ�");
  char destination[10];
  scanf("%10s", destination);
  Order* current = head;
  int hasResult = 0;
  while (current != NULL) {
    if (strcmp(current->destination, destination) == 0) {
      hasResult = 1;
      printf("�ҵ�������%s %s %s %d-%d-%d %d %lf\n", current->name,
             LEVEL_NAME[current->level - 1], current->destination,
             current->startDate.year, current->startDate.month,
             current->startDate.day, current->days, current->amount);
    }
    current = current->next;
  }
  if (!hasResult) {
    printf("δ�ҵ�����\n");
  }
  system("pause");
  return 0;
}
int search_by_start_date() {
  system("cls");
  printf("������Ҫ��ѯ�ĳ������ڣ��� �� �գ���");
  Date startDate;
  scanf("%d %d %d", &startDate.year, &startDate.month, &startDate.day);
  Order* current = head;
  int hasResult = 0;
  while (current != NULL) {
    if (current->startDate.year == startDate.year &&
        current->startDate.month == startDate.month &&
        current->startDate.day == startDate.day) {
      hasResult = 1;
      printf("�ҵ�������%s %s %s %d-%d-%d %d %lf\n", current->name,
             LEVEL_NAME[current->level - 1], current->destination,
             current->startDate.year, current->startDate.month,
             current->startDate.day, current->days, current->amount);
    }
    current = current->next;
  }
  if (!hasResult) {
    printf("δ�ҵ�����\n");
  }
  system("pause");
  return 0;
}
int search_by_days() {
  system("cls");
  printf("������Ҫ��ѯ������������");
  int days;
  scanf("%d", &days);
  Order* current = head;
  int hasResult = 0;
  while (current != NULL) {
    if (current->days == days) {
      hasResult = 1;
      printf("�ҵ�������%s %s %s %d-%d-%d %d %lf\n", current->name,
             LEVEL_NAME[current->level - 1], current->destination,
             current->startDate.year, current->startDate.month,
             current->startDate.day, current->days, current->amount);
    }
    current = current->next;
  }
  if (!hasResult) {
    printf("δ�ҵ�����\n");
  }
  system("pause");
  return 0;
}
int search_by_amount() {
  system("cls");
  printf("������Ҫ��ѯ�Ķ�����");
  double amount;
  scanf("%lf", &amount);
  Order* current = head;
  int hasResult = 0;
  while (current != NULL) {
    if (current->amount == amount) {
      if (!hasResult) {
        hasResult = 1;
        printf("%8s %5s %10s  %10s %8s %8s\n", "����", "�ȼ�", "Ŀ�ĵ�",
               "��������", "��������", "�������");
      }

      printf("%8s %5s %10s  %4d-%02d-%02d %8d %8.2lf\n", current->name,
             LEVEL_NAME[current->level - 1], current->destination,
             current->startDate.year, current->startDate.month,
             current->startDate.day, current->days, current->amount);
    }
    current = current->next;
  }
  if (!hasResult) {
    printf("δ�ҵ�����\n");
  }
  system("pause");
  return 0;
}

int save_order() {
  FILE* file = fopen("data.txt", "w");
  if (file == NULL) {
    printf("�޷����ļ���\n");
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
  printf("�����ѱ��浽data.txt\n");
  return 0;
}

int load_order() {
  printf("���ڴ�data.txt��������...\n");
  release_memory();
  FILE* file = fopen("data.txt", "r");
  if (file == NULL) {
    printf("�޷����ļ���\n");
    system("pause");
    return 0;
  }

  int count = 0;
  while (!feof(file)) {
    Order* new_order = (Order*)malloc(sizeof(Order));
    if (new_order == NULL) {
      printf("�ڴ����ʧ�ܣ�\n");
      break;
    }

    if (fscanf(file, "%s %d %s %d-%d-%d %d %lf", new_order->name,
               &new_order->level, new_order->destination,
               &new_order->startDate.year, &new_order->startDate.month,
               &new_order->startDate.day, &new_order->days,
               &new_order->amount) == 8) {
      // ���½ڵ���뵽����ͷ��
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
  printf("������%d������\n", count);
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
