#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "define.h"
#include "destination.h"
#include "order.h"
#include "utils.h"

Destination *destinations_head = NULL;
Destination *destinations_tail = NULL;
Order *orders_head = NULL;
Order *orders_tail = NULL;

unsigned int destinations_count = 0;
unsigned int orders_count = 0;
int init();

int main() {
  init();
  check_recent_order();
  MenuList menu_list[] = {
      {"订单管理", 1},
      {"目的地管理", 2},
      {"退出", -1},
  };
  int choice = 0;
  while (choice != -1) {
    choice = show_menu("旅游信息管理系统", menu_list,
                       sizeof(menu_list) / sizeof(menu_list[0]));
    switch (choice) {
      case 1:
        order_management();
        break;
      case 2:
        destination_management();
        break;
    }
  }
  release_destination_memory();
  release_order_memory();
  return 0;
}

int init() {
  printf("[*] 初始化中...\n");
  int flag = 0;
  flag |= load_destinations();
  flag |= load_orders();
  if (flag != 0) {
    printf("[-] 初始化时出现异常\n");
    Sleep(3000);
    return -1;
  }
  printf("[+] 初始化完成\n");
  return 0;
}
