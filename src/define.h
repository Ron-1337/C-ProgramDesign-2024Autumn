#ifndef STRUCT_H
#define STRUCT_H

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13

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
  char name[10];
  struct Destination *next;
} Destination;

/* ���� */
typedef struct Order {
  char name[10];
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
