#ifndef STRUCT_H
#define STRUCT_H

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define DESTINATION_NAME_MAX_LENGTH 16
#define ORDER_NAME_MAX_LENGTH 16

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
  char name[DESTINATION_NAME_MAX_LENGTH];
  int time_zone;
  struct Destination *next;
} Destination;

/* 日期和目的地 */
typedef struct DateAndDestination {
  Date date;
  int destination_id;
} DateAndDestination;

/* 订单 */
typedef struct Order {
  char name[ORDER_NAME_MAX_LENGTH];
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
