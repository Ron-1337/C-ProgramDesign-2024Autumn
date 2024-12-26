#include "utils.h"

int checkName(char* name) {
  if (strlen(name) > 8) {
    printf("�������Ȳ��ܳ���8���ַ���\n");
    return 1;
  }
  return 0;
}

int checkLevel(int level) {
  if (level < 1 || level > 4) {
    printf("�ȼ�������1��4֮�䣡\n");
    return 1;
  }
  return 0;
}

int checkDestination(char* destination) {
  if (strlen(destination) > 10) {
    printf("Ŀ�ĵس��Ȳ��ܳ���10���ַ���\n");
    return 1;
  }
  return 0;
}

int checkDate(int year, int month, int day) {
  if (year < 2024 || month < 1 || month > 12 || day < 1 || day > 31) {
    printf("���ڸ�ʽ����\n");
    return 1;
  }
  return 0;
}

int checkDays(int days) {
  if (days < 1 || days > 30) {
    printf("��������������1��30֮�䣡\n");
    return 1;
  }
  return 0;
}

int checkAmount(double amount) {
  if (amount < 0) {
    printf("��������Ϊ����\n");
    return 1;
  }
  return 0;
}