#include "destination.h"

// ���ȫ�ֱ����Ķ���
unsigned long long destination_id_indicator = 0;

int destination_management() {
  int choice = 0;

  while (choice != -1) {
    system("cls");
    MenuList menu[] = {{"Ŀ�ĵ��б�", 1}, {"����Ŀ�ĵ�", 2}, {"���Ŀ�ĵ�", 3},
                       {"ɾ��Ŀ�ĵ�", 4}, {"����Ŀ�ĵ�", 5}, {"����Ŀ�ĵ�", 6},
                       {"����", -1}};
    choice = show_menu("Ŀ�ĵع���", menu, sizeof(menu) / sizeof(menu[0]));
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
        Sleep(1000);
        break;
      case 6:
        save_destinations();
        Sleep(1000);
        break;
    }
  }
  return 0;
}

int show_destinations() {
  printf("=======Ŀ�ĵ��б�=======\n");
  printf("%3s %20s\n", "ID", "����");
  Destination *current = destinations_head;
  while (current != NULL) {
    printf("%3d %20s\n", current->id, current->name);
    current = current->next;
  }
  printf("========================\n");
  system("pause");
  return 0;
}

int search_destination() {
  MenuList menu[] = {{"��ID����", 1}, {"����������", 2}, {"����", -1}};
  int choice = show_menu("����Ŀ�ĵ�", menu, sizeof(menu) / sizeof(menu[0]));
  switch (choice) {
    case 1:
      search_destination_by_id();
      break;
    case 2:
      search_destination_by_name();
      break;
  }
  system("pause");
  return 0;
}

int add_destination() {
  Destination *new_dest = (Destination *)malloc(sizeof(Destination));
  new_dest->next = NULL;  // ��ʼ�� next ָ��

  printf("������Ŀ�ĵ�����: ");
  scanf("%20s", new_dest->name);
  printf("������Ŀ�ĵ�ID: ");
  scanf("%d", &new_dest->id);

  if (check_destination(new_dest)) {
    printf("[-] ���Ŀ�ĵ�ʱ����\n");
    free(new_dest);
    Sleep(3000);
    return -1;
  }

  // ����ID���
  set_destination_id(new_dest->id);

  // ���½ڵ���ӵ�������
  if (destinations_head == NULL) {
    destinations_head = new_dest;
    destinations_tail = new_dest;
  } else {
    destinations_tail->next = new_dest;
    destinations_tail = new_dest;
  }

  save_destinations();

  printf("[+] �����Ŀ�ĵ�%s\n", new_dest->name);
  Sleep(1000);
  return 0;
}

int delete_destination() {
  MenuList menu[] = {{"ͨ��IDɾ��", 1}, {"ͨ������ɾ��", 2}};
  int choice = show_menu("ɾ��Ŀ�ĵ�", menu, sizeof(menu) / sizeof(menu[0]));
  switch (choice) {
    case 1:
      delete_destination_by_id();
      break;
    case 2:
      delete_destination_by_name();
      break;
  }
  save_destinations();
  Sleep(1000);
  return 0;
}

int load_destinations() {
  printf("[*] ���ڼ���Ŀ�ĵ�\n");
  destination_id_indicator = 0;
  release_destination_memory();
  int destinations_count = 0;
  destinations_head = NULL;

  FILE *file = fopen(DESTINATION_FILE, "r");
  if (file == NULL) {
    printf("[-] �޷����ļ�%s\n", DESTINATION_FILE);
    return -1;
  }

  destinations_tail = NULL;
  int id;
  char name[21];

  while (fscanf(file, "%d %20s", &id, name) != EOF) {
    Destination *new_dest = (Destination *)malloc(sizeof(Destination));
    new_dest->id = id;
    strcpy(new_dest->name, name);
    new_dest->next = NULL;

    // ���
    if (check_destination(new_dest) != 0) {
      printf("[-] ����Ŀ�ĵ�ʱ����\n");
      free(new_dest);
      release_destination_memory();
      fclose(file);
      system("pause");
      return -1;
    }

    set_destination_id(new_dest->id);

    if (destinations_head == NULL) {
      destinations_head = new_dest;
      destinations_tail = new_dest;
    } else {
      destinations_tail->next = new_dest;
      destinations_tail = new_dest;
    }
    destinations_count++;
  }

  fclose(file);
  printf("[+] �Ѵ�%s����%d��Ŀ�ĵ�\n", DESTINATION_FILE, destinations_count);
  return 0;
}

int save_destinations() {
  printf("[*] ���ڱ���Ŀ�ĵ�\n");
  int destinations_count = 0;
  Destination *current = destinations_head;

  FILE *file = fopen(DESTINATION_FILE, "w");
  if (file == NULL) {
    printf("[-] �޷����ļ�%s\n", DESTINATION_FILE);
    system("pause");
    return -1;
  }

  while (current != NULL) {
    fprintf(file, "%d %s\n", current->id, current->name);
    current = current->next;
    destinations_count++;
  }

  fclose(file);
  printf("[+] �ѱ���%d��Ŀ�ĵص�%s\n", destinations_count, DESTINATION_FILE);
  return 0;
}

int release_destination_memory() {
  Destination *current = destinations_head;
  Destination *temp;

  while (current != NULL) {
    temp = current;
    current = current->next;
    free(temp);
  }

  destinations_head = NULL;
  destination_id_indicator = 0;
  return 0;
}

/* ���Ŀ�ĵ�
����ֵ: -1,0
���Ϸ�: -1
�Ϸ�: 0 */
int check_destination(Destination *destination) {
  if (check_destination_name(destination->name) != 0) {
    printf("[-] Ŀ�ĵ����Ƴ�����󳤶�(%d)\n", DESTINATION_NAME_MAX_LENGTH);
    return -1;
  }
  switch (check_destination_id(destination->id)) {
    case -1:
      printf("[-] Ŀ�ĵ�ID������Χ\n");
      return -1;
    case 0:
      return 0;
    case 1:
      printf("[-] Ŀ�ĵ�ID�ظ�\n");
      return -1;
  }
  return 0;
}

int search_destination_by_id() {
  int id;
  printf("������Ŀ�ĵ�ID: ");
  scanf("%d", &id);
  switch (check_destination_id(id)) {
    case -1:
      printf("[-] Ŀ�ĵ�ID������Χ\n");
      return -1;
    case 0:
      printf("[-] Ŀ�ĵ�ID������\n");
      return -1;
    case 1:
      Destination *current = destinations_head;
      while (current != NULL && current->id != id) {
        current = current->next;
      }
      printf("Ŀ�ĵ�ID: %d, Ŀ�ĵ�����: %s\n", current->id, current->name);
      return 0;
  }
}

int search_destination_by_name() {
  char name[DESTINATION_NAME_MAX_LENGTH];
  printf("������Ŀ�ĵ�����: ");
  scanf("%20s", name);
  Destination *current = destinations_head;
  while (current != NULL && strcmp(current->name, name) != 0) {
    current = current->next;
  }
  if (current != NULL) {
    printf("Ŀ�ĵ�ID: %d, Ŀ�ĵ�����: %s\n", current->id, current->name);
    return 0;
  } else {
    printf("[-] Ŀ�ĵ����Ʋ�����\n");
    return -1;
  }
}

int delete_destination_by_id() {
  int id;
  printf("������Ŀ�ĵ�ID: ");
  scanf("%d", &id);
  switch (check_destination_id(id)) {
    case -1:
      printf("[-] Ŀ�ĵ�ID������Χ\n");
      return -1;
    case 0:
      printf("[-] Ŀ�ĵ�ID������\n");
      return -1;
    case 1:
      Destination *current = destinations_head;
      Destination *prev = NULL;
      while (current != NULL && current->id != id) {
        prev = current;
        current = current->next;
      }
      if (current == NULL) {
        printf("[-] Ŀ�ĵ�ID������\n");
        return -1;
      }
      if (prev == NULL) {
        destinations_head = current->next;
      } else {
        prev->next = current->next;
      }
      printf("[+] ��ɾ��Ŀ�ĵ�ID: %d, Ŀ�ĵ�����: %s\n", current->id,
             current->name);
      return 0;
  }
}

int delete_destination_by_name() {
  char name[DESTINATION_NAME_MAX_LENGTH];
  printf("������Ŀ�ĵ�����: ");
  scanf("%20s", name);
  Destination *current = destinations_head;
  Destination *prev = NULL;
  while (current != NULL && strcmp(current->name, name) != 0) {
    prev = current;
    current = current->next;
  }
  if (current != NULL) {
    if (prev == NULL) {
      destinations_head = current->next;
    } else {
      prev->next = current->next;
    }
    printf("[+] ��ɾ��Ŀ�ĵ�ID: %d, Ŀ�ĵ�����: %s\n", current->id,
           current->name);
    return 0;
  } else {
    printf("[-] Ŀ�ĵ����Ʋ�����\n");
    return -1;
  }
}

/* ���Ŀ�ĵ�����
����ֵ: -1,0
���ƹ���: -1
�Ϸ�: 0 */
int check_destination_name(char *name) {
  if (strlen(name) > DESTINATION_NAME_MAX_LENGTH) {
    printf("[-] Ŀ�ĵ����Ƴ�����󳤶�(%d)\n", DESTINATION_NAME_MAX_LENGTH);
    return -1;
  }
  return 0;
}

/* ���Ŀ�ĵ�ID
����ֵ: -1,0,1
������Χ: -1
������: 0
����: 1 */
int check_destination_id(int id) {
  if (id < 0 || id > 128) {
    // ������Χ
    return -1;
  }

  unsigned long long bit = 1ULL << id;  // ʹ�� unsigned long long ����
  return (destination_id_indicator & bit) ? 1 : 0;
}

int set_destination_id(int id) {
  unsigned long long bit = 1ULL << id;  // ʹ�� unsigned long long ����
  destination_id_indicator |= bit;
  return 0;
}