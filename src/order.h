#include <stdio.h>
#include <windows.h>

#include "define.h"
#include "utils.h"

#define ORDER_FILE "orders.txt"

extern Order *orders_head;

int order_management();

int save_orders();

int load_orders();
