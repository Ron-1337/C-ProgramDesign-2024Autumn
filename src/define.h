#ifndef STRUCT_H
#define STRUCT_H

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13

/* 等级 */
enum Level { BRONZE = 1, SILVER, GOLD, DIAMOND };

/* 等级名称 */
extern const char *LEVEL_NAME[];

/* 日期 */
typedef struct Date {
  int year;
  int month;
  int day;
} Date;

/* 目的地 */
typedef struct Destination {
  int id;
  char name[10];
  struct Destination *next;
} Destination;

/* 订单 */
typedef struct Order {
  char name[10];
  int level;
  int destination_id;
  Date start_date;
  int duration;
  double price;
  struct Order *next;
} Order;

/* 菜单列表 */
typedef struct MenuList {
  char *name;
  int value;
} MenuList;

#endif
