#include "destination.h"

int destination_management() {
  system("cls");
  MenuList menu[] = {{"Ŀ�ĵ��б�", 1}, {"����Ŀ�ĵ�", 2}, {"���Ŀ�ĵ�", 3},
                     {"ɾ��Ŀ�ĵ�", 4}, {"����Ŀ�ĵ�", 5}, {"����Ŀ�ĵ�", 6},
                     {"����", -1}};
  int choice = show_menu("Ŀ�ĵع���", menu, sizeof(menu) / sizeof(menu[0]));
  switch (choice) {
    case 1:
      show_destinations();
      break;
    case 2:
      search_destination();
      break;
    case 3:
      add_destination();
      break;
    case 4:
      delete_destination();
      break;
    case 5:
      load_destinations();
      break;
    case 6:
      save_destinations();
      break;
  }
  return 0;
}

int show_destinations() {
  printf("=====Ŀ�ĵ��б�=====\n");
  printf("%8s %8s\n", "ID", "����");
  Destination *current = destinations_head;
  while (current != NULL) {
    printf("%8d %8s\n", current->id, current->name);
    current = current->next;
  }
  printf("====================\n");
  system("pause");
  return 0;
}

int search_destination() {}

int add_destination() {}

int delete_destination() {}

int load_destinations() {
  printf("[*] ���ڼ���Ŀ�ĵ�\n");
  int destinations_count = 0;

  printf("[+] �Ѵ�%s����%d��Ŀ�ĵ�\n", DESTINATION_FILE, destinations_count);
  return 0;
}

int save_destinations() {
  printf("[*] ���ڱ���Ŀ�ĵ�\n");
  int destinations_count = 0;

  printf("[+] �ѱ���%d��Ŀ�ĵص�%s\n", destinations_count, DESTINATION_FILE);
  return 0;
}