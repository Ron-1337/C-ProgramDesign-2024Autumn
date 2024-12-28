#include <stdio.h>

#include "define.h"
#include "destination.h"
#include "order.h"
#include "utils.h"

Destination *destinations_head = NULL;
Destination *destinations_tail = NULL;
Order *orders_head = NULL;
Order *orders_tail = NULL;

int init();

int main() {
  init();
  MenuList menu_list[] = {
      {"��������", 1},
      {"Ŀ�ĵع���", 2},
      {"�˳�", -1},
  };
  int choice = 0;
  while (choice != -1) {
    choice = show_menu("������Ϣ����ϵͳ", menu_list,
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
  printf("[*] ��ʼ����...\n");
  int flag = 0;
  flag |= load_destinations();
  flag |= load_orders();
  if (flag != 0) {
    printf("[-] ��ʼ��ʱ�����쳣\n");
    Sleep(1000);
    return -1;
  }
  printf("[+] ��ʼ�����\n");
  Sleep(1000);
  return 0;
}
