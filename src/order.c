#include "order.h"

int order_management() {
  system("cls");
  printf("��������\n");
  system("pause");
  return 0;
}

int save_orders(Order *orders) {
  printf("[*] ���ڱ��涩��\n");
  int orders_count = 0;

  printf("[+] �ѱ���%d��������%s\n", orders_count, ORDER_FILE);
  return 0;
}

int load_orders(Order *orders) {
  printf("[*] ���ڼ��ض���\n");
  int orders_count = 0;

  printf("[+] �Ѵ�%s����%d������\n", ORDER_FILE, orders_count);
  return 0;
}

int release_order_memory() { return 0; }
