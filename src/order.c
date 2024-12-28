#define _CRT_SECURE_NO_WARNINGS

#include "order.h"

int order_management() {
  int choice = 0;
  while (choice != -1) {
    MenuList menu[] = {{"��ʾ����", 1}, {"��������", 2}, {"��Ӷ���", 3},
                       {"ɾ������", 4}, {"���ض���", 5}, {"���涩��", 6},
                       {"����", -1}};
    choice = show_menu("��������", menu, sizeof(menu) / sizeof(menu[0]));
    switch (choice) {
      case 1:
        show_orders();
        break;
      case 2:
        search_order();
        break;
      case 3:
        add_order();
        break;
      case 4:
        delete_order();
        break;
      case 5:
        load_orders();
        Sleep(1000);
        break;
      case 6:
        save_orders();
        Sleep(1000);
        break;
    }
  }
  return 0;
}

int show_orders() {
  printf("========================= �����б� ========================\n");
  printf("%8s %4s %16s %10s %8s %8s\n", "����", "�ȼ�", "Ŀ�ĵ�", "��ʼ����",
         "��������", "�۸�");
  Order *current = orders_head;
  while (current != NULL) {
    printf("%8s %4s %16s %04d-%02d-%02d %8d %8.2f\n", current->name,
           LEVEL_NAME[current->level],
           get_destination_name(current->destination_id),
           current->start_date.year, current->start_date.month,
           current->start_date.day, current->duration, current->price);
    current = current->next;
  }
  printf("===========================================================\n");
  system("pause");
  return 0;
}

int search_order() {
  MenuList menu[] = {{"����������", 1}, {"��Ŀ�ĵ�����", 2}, {"����", -1}};
  int choice =
      show_menu("��ѡ��������ʽ", menu, sizeof(menu) / sizeof(menu[0]));
  switch (choice) {
    case 1:
      search_order_by_name();
      break;
    case 2:
      search_order_by_destination();
      break;
  }
  return 0;
}

int add_order() {
  Order *new_order = (Order *)malloc(sizeof(Order));
  new_order->next = NULL;
  printf("�����붩����Ϣ��\n");
  printf("������");
  char name_format[10];
  sprintf(name_format, "%%%ds", ORDER_NAME_MAX_LENGTH);
  scanf(name_format, new_order->name);

  // ѡ��ȼ�
  MenuList level_menu[] = {{"��ʯ", 3}, {"�ƽ�", 2}, {"����", 1}, {"��ͭ", 0}};
  new_order->level = show_menu("��ѡ��ȼ�", level_menu,
                               sizeof(level_menu) / sizeof(level_menu[0]));
  if (new_order->level == -1) {
    free(new_order);
    return -1;
  }

  // ��Ŀ�ĵ��б���ѡ��Ŀ�ĵ�
  MenuList *destination_menu =
      (MenuList *)malloc(sizeof(MenuList) * (destinations_count));
  Destination *current = destinations_head;
  int i = 0;
  while (current != NULL) {
    destination_menu[i].name = current->name;
    destination_menu[i].value = current->id;
    current = current->next;
    i++;
  }
  new_order->destination_id =
      show_menu("��ѡ��Ŀ�ĵ�", destination_menu, destinations_count);
  free(destination_menu);

  printf("��ʼ����(YYYY-MM-DD)��");
  scanf("%d%d%d", &new_order->start_date.year, &new_order->start_date.month,
        &new_order->start_date.day);
  printf("����������");
  scanf("%d", &new_order->duration);
  printf("�۸�");
  scanf("%lf", &new_order->price);

  if (check_order(new_order)) {
    printf("[-] ��Ӷ���ʱ����\n");
    free(new_order);
    Sleep(3000);
    return -1;
  }

  if (orders_head == NULL) {
    orders_head = new_order;
    orders_tail = new_order;
  } else {
    orders_tail->next = new_order;
    orders_tail = new_order;
  }

  orders_count++;
  save_orders();
  printf("[+] ����Ӷ���\n");
  Sleep(1000);
  return 0;
}

int delete_order() {
  printf("[*] ����ɾ������\n");
  Sleep(1000);
  return 0;
}

int load_orders() {
  printf("[*] ���ڼ��ض���\n");
  release_order_memory();
  int orders_count = 0;

  FILE *file = fopen(ORDER_FILE, "r");
  if (file == NULL) {
    printf("[-] �޷����ļ�%s\n", ORDER_FILE);
    return -1;
  }

  Order *new_order = (Order *)malloc(sizeof(Order));
  while (fscanf(file, "%s %d %d %d %d %d %d %lf", new_order->name,
                &new_order->level, &new_order->destination_id,
                &new_order->start_date.year, &new_order->start_date.month,
                &new_order->start_date.day, &new_order->duration,
                &new_order->price) != EOF) {
    new_order->next = NULL;

    if (check_order(new_order)) {
      printf("[-] �������ݴ���\n");
      free(new_order);
      fclose(file);
      return -1;
    }

    orders_count++;

    if (orders_head == NULL) {
      orders_head = new_order;
      orders_tail = new_order;
    } else {
      orders_tail->next = new_order;
      orders_tail = new_order;
    }
    new_order = (Order *)malloc(sizeof(Order));
  }

  fclose(file);
  printf("[+] �Ѵ�%s����%d������\n", ORDER_FILE, orders_count);
  return 0;
}

int save_orders() {
  printf("[*] ���ڱ��涩��\n");
  int orders_count = 0;
  FILE *file = fopen(ORDER_FILE, "w");
  if (file == NULL) {
    printf("[-] �޷����ļ�%s\n", ORDER_FILE);
    fclose(file);
    return -1;
  }

  Order *current = orders_head;
  while (current != NULL) {
    orders_count++;
    fprintf(file, "%s %d %d %d %d %d %d %.2lf\n", current->name, current->level,
            current->destination_id, current->start_date.year,
            current->start_date.month, current->start_date.day,
            current->duration, current->price);
    current = current->next;
  }
  fclose(file);
  printf("[+] �ѱ���%d��������%s\n", orders_count, ORDER_FILE);
  return 0;
}

int check_order(Order *order) {
  // printf("%s %d %d %d %d %d %d %.2lf\n", order->name, order->level,
  //        order->destination_id, order->start_date.year,
  //        order->start_date.month, order->start_date.day, order->duration,
  //        order->price);
  if (strlen(order->name) > ORDER_NAME_MAX_LENGTH) {
    printf("[-] ������������\n");
    return -1;
  }
  if (order->level < 0 || order->level >= 4) {
    printf("[-] �����ȼ�����\n");
    return -1;
  }
  if (check_destination_id(order->destination_id) != 1) {
    printf("[-] ����Ŀ�ĵش���\n");
    return -1;
  }
  if (order->start_date.year < 2024 || order->start_date.month < 1 ||
      order->start_date.month > 12 || order->start_date.day < 1 ||
      order->start_date.day > 31) {
    printf("[-] ������ʼ���ڴ���\n");
    return -1;
  }
  if (order->duration < 1 || order->duration > 30) {
    printf("[-] ����������������\n");
    return -1;
  }
  if (order->price < 0) {
    printf("[-] �����۸����\n");
    return -1;
  }
  return 0;
}

int release_order_memory() {
  Order *current = orders_head;
  Order *next = NULL;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
  orders_head = NULL;
  orders_tail = NULL;
  return 0;
}

int search_order_by_name() {
  printf("[*] ���ڰ�������������\n");
  return 0;
}

int search_order_by_destination() {
  printf("[*] ���ڰ�Ŀ�ĵ���������\n");
  return 0;
}
