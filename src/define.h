#ifndef STRUCT_H
#define STRUCT_H

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define DESTINATION_NAME_MAX_LENGTH 16
#define ORDER_NAME_MAX_LENGTH 16

/* �ȼ� */
enum Level { BRONZE = 1, SILVER, GOLD, DIAMOND };

/* �ȼ����� */
extern const char *LEVEL_NAME[];

/* ���� */
typedef struct Date {
  int year;
  int month;
  int day;
} Date;

/* Ŀ�ĵ� */
typedef struct Destination {
  int id;
  char name[DESTINATION_NAME_MAX_LENGTH];
  int time_zone;
  struct Destination *next;
} Destination;

/* ���ں�Ŀ�ĵ� */
typedef struct DateAndDestination {
  Date date;
  int destination_id;
} DateAndDestination;

/* ���� */
typedef struct Order {
  char name[ORDER_NAME_MAX_LENGTH];
  int level;
  int destination_id;
  Date start_date;
  int duration;
  double price;
  struct Order *next;
} Order;

/* �˵��б� */
typedef struct MenuList {
  char *name;
  int value;
} MenuList;

#endif
