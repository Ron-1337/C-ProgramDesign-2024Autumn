#include "order.h"

int order_management() {
  system("cls");
  printf("订单管理\n");
  system("pause");
  return 0;
}

int save_orders(Order *orders) {
  printf("[*] 正在保存订单\n");
  int orders_count = 0;

  printf("[+] 已保存%d条订单到%s\n", orders_count, ORDER_FILE);
  return 0;
}

int load_orders(Order *orders) {
  printf("[*] 正在加载订单\n");
  int orders_count = 0;

  printf("[+] 已从%s加载%d条订单\n", ORDER_FILE, orders_count);
  return 0;
}

int release_order_memory() { return 0; }
