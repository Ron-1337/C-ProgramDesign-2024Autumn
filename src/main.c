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
      {"订单管理", 1}, {"目的地管理", 2}, {"最近订单", 3},
      {"天气查询", 4}, {"目的地时间", 5}, {"退出", -1},
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

int get_weather() {
  int choice = choose_destination();
  if (choice == -1) {
    return -1;
  }

  Destination* des = get_destination_by_id(choice);
  if (des == NULL) {
    printf("[-] 未找到目的地信息\n");
    system("pause");
    return -1;
  }

  // system("cls");
  printf("[*] 正在获取天气...\r");

  const wchar_t* server = L"api.zzpeng.com";

  // 构建请求路径
  wchar_t path[256];
  wchar_t location[128];
  MultiByteToWideChar(CP_ACP, 0, des->name, -1, location,
                      sizeof(location) / sizeof(wchar_t));
  swprintf(path, sizeof(path) / sizeof(wchar_t), L"/weather?location=%ls",
           location);

  size_t resultSize = 0;
  char* result = http_get(server, path, &resultSize);

  if (result) {
    printf("[+] 获取天气成功    \n");
    printf("%s 七日天气\n%s\n", des->name, result);
    free(result);
  } else {
    printf("[-] 获取天气失败\n");
  }
  system("pause");
  return 0;
}