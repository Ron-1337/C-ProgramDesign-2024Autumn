#define _CRT_SECURE_NO_WARNINGS

#include "order.h"

int order_management() {
  int choice = 0;
  while (choice != -1) {
    MenuList menu[] = {{"统计订单", 1}, {"搜索订单", 2}, {"添加订单", 3},
                       {"删除订单", 4}, {"加载订单", 5}, {"保存订单", 6},
                       {"返回", -1}};
    choice = show_menu("订单管理", menu, sizeof(menu) / sizeof(menu[0]));
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
  MenuList menu[] = {
      {"按用户等级排序", 1}, {"按订单金额排序", 2}, {"返回", -1}};
  int choice = show_menu("订单统计", menu, sizeof(menu) / sizeof(menu[0]));
  switch (choice) {
    case 1:
      sort_by_price();
      sort_by_level();
      break;

    case 2:
      sort_by_level();
      sort_by_price();
      break;
    case -1:
      return -1;
  }

  list_orders(show_rules_all, NULL);
  return 0;
}

int check_recent_order() {
  system("cls");
  Order *current = orders_head;
  int count = 0;
  while (current != NULL) {
    int diff = time_diff(current->start_date);
    if (diff <= current->duration && diff >= -1) {
      count++;
      printf("[+] 最近订单: %s %s %d %d-%d-%d %d %.2f\n", current->name,
             LEVEL_NAME[current->level], current->destination_id,
             current->start_date.year, current->start_date.month,
             current->start_date.day, current->duration, current->price);
    }
    current = current->next;
  }
  if (count == 0) {
    printf("[-] 没有最近订单\n");
  } else {
    printf("[+] 最近订单数量: %d\n", count);
  }
  return 0;
}

int search_order() {
  MenuList menu[] = {{"按等级搜索(今天)", 1},   {"按日期搜索", 2},
                     {"按姓名搜索", 3},         {"按目的地搜索", 4},
                     {"按日期和目的地搜索", 5}, {"返回", -1}};
  int choice =
      show_menu("请选择搜索方式", menu, sizeof(menu) / sizeof(menu[0]));
  switch (choice) {
    case 1:
      search_order_by_level();
      break;
    case 2:
      search_order_by_date();
      break;
    case 3:
      search_order_by_name();
      break;
    case 4:
      search_order_by_destination();
      break;
    case 5:
      search_order_by_date_and_destination();
      break;
    case -1:
      return -1;
  }
  return 0;
}

int add_order() {
  Order *new_order = (Order *)malloc(sizeof(Order));
  new_order->next = NULL;
  printf("请输入订单信息：\n");
  printf("姓名：");
  char name_format[10];
  sprintf(name_format, "%%%ds", ORDER_NAME_MAX_LENGTH);
  scanf(name_format, new_order->name);

  // 选择等级
  MenuList level_menu[] = {{"钻石", 3}, {"黄金", 2}, {"白银", 1}, {"青铜", 0}};
  new_order->level = show_menu("请选择等级", level_menu,
                               sizeof(level_menu) / sizeof(level_menu[0]));
  if (new_order->level == -1) {
    free(new_order);
    return -1;
  }

  new_order->destination_id = choose_destination();

  printf("开始日期(YYYY-MM-DD)：");
  scanf("%d%d%d", &new_order->start_date.year, &new_order->start_date.month,
        &new_order->start_date.day);
  printf("持续天数：");
  scanf("%d", &new_order->duration);
  printf("价格：");
  scanf("%lf", &new_order->price);

  if (check_order(new_order)) {
    printf("[-] 添加订单时出错\n");
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
  printf("[+] 已添加订单\n");
  Sleep(1000);
  return 0;
}

int delete_order() {
  printf("[*] 正在删除订单\n");
  Sleep(1000);
  return 0;
}

int load_orders() {
  printf("[*] 正在加载订单\n");
  release_order_memory();
  int orders_count = 0;

  FILE *file = fopen(ORDER_FILE, "r");
  if (file == NULL) {
    printf("[-] 无法打开文件%s\n", ORDER_FILE);
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
      printf("[-] 订单数据错误\n");
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
  printf("[+] 已从%s加载%d条订单\n", ORDER_FILE, orders_count);
  return 0;
}

int save_orders() {
  printf("[*] 正在保存订单\n");
  int orders_count = 0;
  FILE *file = fopen(ORDER_FILE, "w");
  if (file == NULL) {
    printf("[-] 无法打开文件%s\n", ORDER_FILE);
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
  printf("[+] 已保存%d条订单到%s\n", orders_count, ORDER_FILE);
  return 0;
}

int check_order(Order *order) {
  // printf("%s %d %d %d %d %d %d %.2lf\n", order->name, order->level,
  //        order->destination_id, order->start_date.year,
  //        order->start_date.month, order->start_date.day, order->duration,
  //        order->price);
  if (strlen(order->name) > ORDER_NAME_MAX_LENGTH) {
    printf("[-] 订单姓名过长\n");
    return -1;
  }
  if (order->level < 0 || order->level >= 4) {
    printf("[-] 订单等级错误\n");
    return -1;
  }
  if (check_destination_id(order->destination_id) != 1) {
    printf("[-] 订单目的地错误\n");
    return -1;
  }
  if (order->start_date.year < 2024 || order->start_date.month < 1 ||
      order->start_date.month > 12 || order->start_date.day < 1 ||
      order->start_date.day > 31) {
    printf("[-] 订单开始日期错误\n");
    return -1;
  }
  if (order->duration < 1 || order->duration > 30) {
    printf("[-] 订单持续天数错误\n");
    return -1;
  }
  if (order->price < 0) {
    printf("[-] 订单价格错误\n");
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
  printf("输入你要查询的姓名: ");
  char name[ORDER_NAME_MAX_LENGTH];
  scanf("%s", name);
  list_orders(show_rules_by_name, name);
  return 0;
}

int search_order_by_destination() {
  int destination = choose_destination();
  sort_by_price();
  list_orders(show_rules_by_destination, &destination);
  return 0;
}

int search_order_by_date_and_destination() {
  DateAndDestination date_and_destination;
  date_and_destination.destination_id = choose_destination();
  printf("请输入你要查询的日期(YYYY-MM-DD): ");
  scanf("%d%d%d", &date_and_destination.date.year,
        &date_and_destination.date.month, &date_and_destination.date.day);
  sort_by_level();
  list_orders(show_rules_by_date_and_destination, &date_and_destination);
  return 0;
}

int search_order_by_level() {
  MenuList menu[] = {
      {"钻石", 3}, {"黄金", 2}, {"白银", 1}, {"青铜", 0}, {"返回", -1}};
  int choice = show_menu("请选择等级", menu, sizeof(menu) / sizeof(menu[0]));
  if (choice == -1) {
    return -1;
  }
  list_orders(show_rules_today_by_level, &choice);
  return 0;
}

int search_order_by_date() {
  printf("请输入你要查询的日期(YYYY-MM-DD): ");
  Date date;
  scanf("%d%d%d", &date.year, &date.month, &date.day);

  sort_by_level();
  list_orders(show_rules_by_date, &date);
  return 0;
}

// 排序函数 传入比较函数
int sort_orders(CompareFunc compare) {
  if (orders_head == NULL || orders_head->next == NULL) {
    return 0;  // 空链表或只有一个节点无需排序
  }

  int swapped;
  Order *ptr1;
  Order *lptr = NULL;

  do {
    swapped = 0;
    ptr1 = orders_head;

    while (ptr1->next != lptr) {
      if (compare(ptr1, ptr1->next) > 0) {
        // 交换节点数据
        char temp_name[ORDER_NAME_MAX_LENGTH];
        strcpy(temp_name, ptr1->name);
        strcpy(ptr1->name, ptr1->next->name);
        strcpy(ptr1->next->name, temp_name);

        int temp_level = ptr1->level;
        ptr1->level = ptr1->next->level;
        ptr1->next->level = temp_level;

        int temp_destination_id = ptr1->destination_id;
        ptr1->destination_id = ptr1->next->destination_id;
        ptr1->next->destination_id = temp_destination_id;

        Date temp_date = ptr1->start_date;
        ptr1->start_date = ptr1->next->start_date;
        ptr1->next->start_date = temp_date;

        int temp_duration = ptr1->duration;
        ptr1->duration = ptr1->next->duration;
        ptr1->next->duration = temp_duration;

        double temp_price = ptr1->price;
        ptr1->price = ptr1->next->price;
        ptr1->next->price = temp_price;

        swapped = 1;
      }
      ptr1 = ptr1->next;
    }
    lptr = ptr1;
  } while (swapped);

  return 0;
}

// 比较函数：按等级从高到低排序
int compare_by_level(Order *a, Order *b) { return b->level - a->level; }

// 比较函数：按价格从高到低排序
int compare_by_price(Order *a, Order *b) { return b->price - a->price; }

// 比较函数：按日期从早到晚排序
int compare_by_date(Order *a, Order *b) {
  return (a->start_date.year * 10000 + a->start_date.month * 100 +
          a->start_date.day) -
         (b->start_date.year * 10000 + b->start_date.month * 100 +
          b->start_date.day);
}

// 按等级排序的包装函数
int sort_by_level() { return sort_orders(compare_by_level); }

// 按价格排序的包装函数
int sort_by_price() { return sort_orders(compare_by_price); }

// 按开始日期排序的包装函数
int sort_by_date() { return sort_orders(compare_by_date); }

int show_rules_all(Order *order, void *arg) { return 1; }

int show_rules_today_by_level(Order *order, void *arg) {
  return order->level == *(int *)arg && time_diff(order->start_date) == 0;
}

int show_rules_by_date(Order *order, void *arg) {
  return order->start_date.year == ((Date *)arg)->year &&
         order->start_date.month == ((Date *)arg)->month &&
         order->start_date.day == ((Date *)arg)->day;
}

int show_rules_by_name(Order *order, void *arg) {
  return strcmp(order->name, (char *)arg) == 0;
}

int show_rules_by_destination(Order *order, void *arg) {
  return order->destination_id == *(int *)arg;
}

int show_rules_by_date_and_destination(Order *order, void *arg) {
  DateAndDestination *date_and_destination = (DateAndDestination *)arg;
  return order->start_date.year == date_and_destination->date.year &&
         order->start_date.month == date_and_destination->date.month &&
         order->start_date.day == date_and_destination->date.day &&
         order->destination_id == date_and_destination->destination_id;
}

int list_orders(int (*show_rules)(Order *, void *), void *arg) {
  int count = 0;

  Order *current = orders_head;
  while (current != NULL) {
    if (show_rules(current, arg)) {
      if (count == 0) {
        system("cls");
        printf("========================= 订单统计 ========================\n");
        printf("%8s %4s %16s %10s %8s %8s\n", "姓名", "等级", "目的地",
               "开始日期", "持续天数", "价格");
      }
      printf("%8s %4s %16s %04d-%02d-%02d %8d %8.2f\n", current->name,
             LEVEL_NAME[current->level],
             get_destination_name(current->destination_id),
             current->start_date.year, current->start_date.month,
             current->start_date.day, current->duration, current->price);
      count++;
    }
    current = current->next;
  }

  if (count != 0) {
    printf("===========================================================\n");
  } else {
    printf("[-] 没有找到符合条件的订单\n");
  }
  system("pause");
  return 0;
}
