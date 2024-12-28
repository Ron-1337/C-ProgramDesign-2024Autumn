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

// ����ȽϺ���������
typedef int (*CompareFunc)(Order*, Order*);

int order_management();

// ͨ��������
int sort_orders(CompareFunc compare);

// ����ıȽϺ���
int compare_by_level(Order* a, Order* b);
int compare_by_price(Order* a, Order* b);

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
int sort_by_level();
int sort_by_price();
