#define _CRT_SECURE_NO_WARNINGS

#include "destination.h"

// 添加全局变量的定义
unsigned long long destination_id_indicator = 0;

int destination_management() {
  int choice = 0;

  while (choice != -1) {
    system("cls");
    MenuList menu[] = {{"目的地列表", 1}, {"搜索目的地", 2}, {"添加目的地", 3},
                       {"删除目的地", 4}, {"加载目的地", 5}, {"保存目的地", 6},
                       {"返回", -1}};
    choice = show_menu("目的地管理", menu, sizeof(menu) / sizeof(menu[0]));
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
        Sleep(1000);
        break;
      case 6:
        save_destinations();
        Sleep(1000);
        break;
    }
  }
  return 0;
}

int show_destinations() {
  printf("=====目的地列表=====\n");
  printf("%3s %16s\n", "ID", "名称");
  Destination *current = destinations_head;
  while (current != NULL) {
    printf("%3d %16s\n", current->id, current->name);
    current = current->next;
  }
  printf("====================\n");
  system("pause");
  return 0;
}

int search_destination() {
  MenuList menu[] = {{"按ID搜索", 1}, {"按名称搜索", 2}, {"返回", -1}};
  int choice = show_menu("搜索目的地", menu, sizeof(menu) / sizeof(menu[0]));
  switch (choice) {
    case 1:
      search_destination_by_id();
      break;
    case 2:
      search_destination_by_name();
      break;
  }
  system("pause");
  return 0;
}

int add_destination() {
  Destination *new_dest = (Destination *)malloc(sizeof(Destination));
  new_dest->next = NULL;  // 初始化 next 指针

  printf("请输入目的地名称: ");
  char name_format[10];
  sprintf(name_format, "%%%ds", DESTINATION_NAME_MAX_LENGTH);
  scanf(name_format, new_dest->name);
  getchar();  // 清除输入缓冲区中的换行符
  printf("请输入目的地ID(0-127): ");
  scanf("%d", &new_dest->id);

  if (check_destination(new_dest)) {
    printf("[-] 添加目的地时出错\n");
    free(new_dest);
    Sleep(3000);
    return -1;
  }

  // 设置ID标记
  set_destination_id(new_dest->id);

  // 将新节点添加到链表中
  if (destinations_head == NULL) {
    destinations_head = new_dest;
    destinations_tail = new_dest;
  } else {
    destinations_tail->next = new_dest;
    destinations_tail = new_dest;
  }

  save_destinations();

  printf("[+] 已添加目的地%s\n", new_dest->name);
  Sleep(1000);
  return 0;
}

int delete_destination() {
  MenuList menu[] = {{"通过ID删除", 1}, {"通过名称删除", 2}};
  int choice = show_menu("删除目的地", menu, sizeof(menu) / sizeof(menu[0]));
  switch (choice) {
    case 1:
      delete_destination_by_id();
      break;
    case 2:
      delete_destination_by_name();
      break;
  }
  save_destinations();
  Sleep(1000);
  return 0;
}

int load_destinations() {
  printf("[*] 正在加载目的地\n");
  release_destination_memory();
  destinations_count = 0;

  FILE *file = fopen(DESTINATION_FILE, "r");
  if (file == NULL) {
    printf("[-] 无法打开文件%s\n", DESTINATION_FILE);
    fclose(file);
    return -1;
  }

  int id;
  char name[21];

  while (fscanf(file, "%d %16s", &id, name) != EOF) {
    Destination *new_dest = (Destination *)malloc(sizeof(Destination));
    new_dest->id = id;
    strcpy(new_dest->name, name);
    new_dest->next = NULL;

    // 检查
    if (check_destination(new_dest) != 0) {
      printf("[-] 加载目的地时出错\n");
      free(new_dest);
      release_destination_memory();
      fclose(file);
      system("pause");
      return -1;
    }

    set_destination_id(new_dest->id);

    if (destinations_head == NULL) {
      destinations_head = new_dest;
      destinations_tail = new_dest;
    } else {
      destinations_tail->next = new_dest;
      destinations_tail = new_dest;
    }
    destinations_count++;
  }

  fclose(file);
  printf("[+] 已从%s加载%d条目的地\n", DESTINATION_FILE, destinations_count);
  return 0;
}

int save_destinations() {
  printf("[*] 正在保存目的地\n");
  destinations_count = 0;
  Destination *current = destinations_head;

  FILE *file = fopen(DESTINATION_FILE, "w");
  if (file == NULL) {
    printf("[-] 无法打开文件%s\n", DESTINATION_FILE);
    system("pause");
    return -1;
  }

  while (current != NULL) {
    fprintf(file, "%d %s\n", current->id, current->name);
    current = current->next;
    destinations_count++;
  }

  fclose(file);
  printf("[+] 已保存%d条目的地到%s\n", destinations_count, DESTINATION_FILE);
  return 0;
}

int release_destination_memory() {
  Destination *current = destinations_head;
  Destination *temp;

  while (current != NULL) {
    temp = current;
    current = current->next;
    free(temp);
  }

  destinations_head = NULL;
  destinations_tail = NULL;
  destination_id_indicator = 0;
  return 0;
}

/* 检查目的地
返回值: -1,0
不合法: -1
合法: 0 */
int check_destination(Destination *destination) {
  if (check_destination_name(destination->name) != 0) {
    printf("[-] 目的地名称超过最大长度(%d)\n", DESTINATION_NAME_MAX_LENGTH);
    return -1;
  }
  switch (check_destination_id(destination->id)) {
    case -1:
      printf("[-] 目的地ID超出范围\n");
      return -1;
    case 0:
      return 0;
    case 1:
      printf("[-] 目的地ID重复\n");
      return -1;
  }
  return 0;
}

int search_destination_by_id() {
  int id;
  printf("请输入目的地ID(0-127): ");
  scanf("%d", &id);
  Destination *current = destinations_head;
  switch (check_destination_id(id)) {
    case -1:
      printf("[-] 目的地ID超出范围\n");
      return -1;
    case 0:
      printf("[-] 目的地ID不存在\n");
      return -1;
    case 1:
      while (current != NULL && current->id != id) {
        current = current->next;
      }
      printf("目的地ID: %d, 目的地名称: %s\n", current->id, current->name);
      return 0;
  }
}

int search_destination_by_name() {
  char name[DESTINATION_NAME_MAX_LENGTH];
  printf("请输入目的地名称: ");
  scanf("%20s", name);
  Destination *current = destinations_head;
  while (current != NULL && strcmp(current->name, name) != 0) {
    current = current->next;
  }
  if (current != NULL) {
    printf("目的地ID: %d, 目的地名称: %s\n", current->id, current->name);
    return 0;
  } else {
    printf("[-] 目的地名称不存在\n");
    return -1;
  }
}

int delete_destination_by_id() {
  int id;
  printf("请输入目的地ID: ");
  scanf("%d", &id);

  Destination *current = destinations_head;
  Destination *prev = NULL;

  switch (check_destination_id(id)) {
    case -1:
      printf("[-] 目的地ID超出范围\n");
      return -1;
    case 0:
      printf("[-] 目的地ID不存在\n");
      return -1;
    case 1:
      while (current != NULL && current->id != id) {
        prev = current;
        current = current->next;
      }
      if (current == NULL) {
        printf("[-] 目的地ID不存在\n");
        return -1;
      }
      if (destination_is_used(current->id)) {
        printf("[-] 目的地ID被占用\n");
        return -1;
      }
      if (prev == NULL) {
        destinations_head = current->next;
      } else {
        prev->next = current->next;
      }
      if (current == destinations_tail) {
        destinations_tail = prev;
      }
      printf("[+] 已删除目的地ID: %d, 目的地名称: %s\n", current->id,
             current->name);
      free(current);
      return 0;
  }
  return -1;
}

int delete_destination_by_name() {
  char name[DESTINATION_NAME_MAX_LENGTH];
  printf("请输入目的地名称: ");
  scanf("%20s", name);
  Destination *current = destinations_head;
  Destination *prev = NULL;
  while (current != NULL && strcmp(current->name, name) != 0) {
    prev = current;
    current = current->next;
  }
  if (current == NULL) {
    printf("[-] 目的地ID不存在\n");
    return -1;
  }
  if (destination_is_used(current->id)) {
    printf("[-] 目的地ID被占用\n");
    return -1;
  }
  if (prev == NULL) {
    destinations_head = current->next;
  } else {
    prev->next = current->next;
  }
  if (current == destinations_tail) {
    destinations_tail = prev;
  }
  printf("[+] 已删除目的地ID: %d, 目的地名称: %s\n", current->id,
         current->name);
  return 0;
}

char *get_destination_name(int id) {
  Destination *current = destinations_head;
  while (current != NULL && current->id != id) {
    current = current->next;
  }
  if (current == NULL) {
    return NULL;
  }
  return current->name;
}

/* 检查目的地名称
返回值: -1,0
名称过长: -1
合法: 0 */
int check_destination_name(char *name) {
  if (strlen(name) > DESTINATION_NAME_MAX_LENGTH) {
    printf("[-] 目的地名称超过最大长度(%d)\n", DESTINATION_NAME_MAX_LENGTH);
    return -1;
  }
  return 0;
}

/* 检查目的地ID
返回值: -1,0,1
超出范围: -1
不存在: 0
存在: 1 */
int check_destination_id(int id) {
  if (id < 0 || id > 128) {
    // 超出范围
    return -1;
  }

  unsigned long long bit = 1ULL << id;
  return (destination_id_indicator & bit) ? 1 : 0;
}

int set_destination_id(int id) {
  unsigned long long bit = 1ULL << id;
  destination_id_indicator |= bit;
  return 0;
}

int destination_is_used(int id) {
  Order *current = orders_head;
  while (current != NULL) {
    if (current->destination_id == id) {
      return 1;
    }
    current = current->next;
  }
  return 0;
}