#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "define.h"
#include "destination.h"
#include "utils.h"

#define ORDER_FILE "orders.txt"

extern Order* orders_head;
extern Order* orders_tail;

extern unsigned int orders_count;
extern unsigned int destinations_count;

// 定义比较函数的类型
typedef int (*CompareFunc)(Order*, Order*);

int order_management();

int check_recent_order();

// 比较函数声明
int compare_by_level(Order* a, Order* b);
int compare_by_price(Order* a, Order* b);
int compare_by_date(Order* a, Order* b);

// 通用排序函数
int sort_orders(CompareFunc compare);

int show_orders();
int search_order();
int add_order();
int delete_order();
int load_orders();
int save_orders();

int check_order(Order* order);
int release_order_memory();
int search_order_by_name();
int search_order_by_destination();
int search_order_by_date_and_destination();
int search_order_by_level();
int search_order_by_date();
int sort_by_level();
int sort_by_price();
int sort_by_date();

// 显示规则函数声明
int show_rules_all(Order* order, void* arg);
int show_rules_today_by_level(Order* order, void* arg);
int show_rules_by_date(Order* order, void* arg);
int show_rules_by_name(Order* order, void* arg);
int show_rules_by_destination(Order* order, void* arg);
int show_rules_by_date_and_destination(Order* order, void* arg);

int list_orders(int (*show_rules)(Order*, void*), void* arg);
