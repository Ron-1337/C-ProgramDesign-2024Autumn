#include "destination.h"

int destination_management() {
  system("cls");
  MenuList menu[] = {{"目的地列表", 1}, {"搜索目的地", 2}, {"添加目的地", 3},
                     {"删除目的地", 4}, {"加载目的地", 5}, {"保存目的地", 6},
                     {"返回", -1}};
  int choice = show_menu("目的地管理", menu, sizeof(menu) / sizeof(menu[0]));
  switch (choice) {
    case 1:
      show_destinations();
      break;
    case 2:
      search_destination();
      break;
    case 3:
      add_destination();
      break;
    case 4:
      delete_destination();
      break;
    case 5:
      load_destinations();
      break;
    case 6:
      save_destinations();
      break;
  }
  return 0;
}

int show_destinations() {
  printf("=====目的地列表=====\n");
  printf("%8s %8s\n", "ID", "名称");
  Destination *current = destinations_head;
  while (current != NULL) {
    printf("%8d %8s\n", current->id, current->name);
    current = current->next;
  }
  printf("====================\n");
  system("pause");
  return 0;
}

int search_destination() {}

int add_destination() {}

int delete_destination() {}

int load_destinations() {
  printf("[*] 正在加载目的地\n");
  int destinations_count = 0;

  printf("[+] 已从%s加载%d条目的地\n", DESTINATION_FILE, destinations_count);
  return 0;
}

int save_destinations() {
  printf("[*] 正在保存目的地\n");
  int destinations_count = 0;

  printf("[+] 已保存%d条目的地到%s\n", destinations_count, DESTINATION_FILE);
  return 0;
}