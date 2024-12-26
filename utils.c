#include "utils.h"

int checkName(char* name) {
  if (strlen(name) > 8) {
    printf("姓名长度不能超过8个字符！\n");
    return 1;
  }
  return 0;
}

int checkLevel(int level) {
  if (level < 1 || level > 4) {
    printf("等级必须在1到4之间！\n");
    return 1;
  }
  return 0;
}

int checkDestination(char* destination) {
  if (strlen(destination) > 10) {
    printf("目的地长度不能超过10个字符！\n");
    return 1;
  }
  return 0;
}

int checkDate(int year, int month, int day) {
  if (year < 2024 || month < 1 || month > 12 || day < 1 || day > 31) {
    printf("日期格式错误！\n");
    return 1;
  }
  return 0;
}

int checkDays(int days) {
  if (days < 1 || days > 30) {
    printf("旅游天数必须在1到30之间！\n");
    return 1;
  }
  return 0;
}

int checkAmount(double amount) {
  if (amount < 0) {
    printf("订单金额不能为负！\n");
    return 1;
  }
  return 0;
}