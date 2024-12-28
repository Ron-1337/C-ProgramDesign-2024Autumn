#include <stdio.h>

#include "define.h"
#include "destination.h"
#include "order.h"
#include "utils.h"

Destination *destinations_head = NULL;
Order *orders_head = NULL;

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
  return 0;
}

int init() {
  printf("[*] ��ʼ����...\n");
  load_destinations();
  load_orders();
  printf("[+] ��ʼ�����\n");
  system("pause");
  return 0;
}
