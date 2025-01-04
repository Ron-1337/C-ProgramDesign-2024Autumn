#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "define.h"
#include "destination.h"
#include "order.h"
#include "utils.h"

Destination* destinations_head = NULL;
Destination* destinations_tail = NULL;
Order* orders_head = NULL;
Order* orders_tail = NULL;

unsigned int destinations_count = 0;
unsigned int orders_count = 0;

int init();
void test();
int get_weather();

int main() {
  init();
  check_recent_order();
  Sleep(1000);
  MenuList menu_list[] = {
      {"��������", 1}, {"Ŀ�ĵع���", 2}, {"�������", 3},
      {"������ѯ", 4}, {"Ŀ�ĵ�ʱ��", 5}, {"�˳�", -1},
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
      case 3:
        check_recent_order();
        system("pause");
        break;
      case 4:
        get_weather();
        break;
      case 5:
        destination_time();
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
    Sleep(3000);
    return -1;
  }
  printf("[+] ��ʼ�����\n");
  return 0;
}

int get_weather() {
  int choice = choose_destination();
  if (choice == -1) {
    return -1;
  }

  Destination* des = get_destination_by_id(choice);
  if (des == NULL) {
    printf("[-] δ�ҵ�Ŀ�ĵ���Ϣ\n");
    system("pause");
    return -1;
  }

  // system("cls");
  printf("[*] ���ڻ�ȡ����...\r");

  const wchar_t* server = L"api.zzpeng.com";

  // ��������·��
  wchar_t path[256];
  wchar_t location[128];
  MultiByteToWideChar(CP_ACP, 0, des->name, -1, location,
                      sizeof(location) / sizeof(wchar_t));
  swprintf(path, sizeof(path) / sizeof(wchar_t), L"/weather?location=%ls",
           location);

  size_t resultSize = 0;
  char* result = http_get(server, path, &resultSize);

  if (result) {
    printf("[+] ��ȡ�����ɹ�    \n");
    printf("%s ��������\n%s\n", des->name, result);
    free(result);
  } else {
    printf("[-] ��ȡ����ʧ��\n");
  }
  system("pause");
  return 0;
}