#include <stdio.h>
#include <windows.h>

#include "define.h"
#include "utils.h"

#define DESTINATION_FILE "destinations.txt"

extern Destination *destinations_head;
extern Destination *destinations_tail;

extern unsigned long long destination_id_indicator;

int destination_management();

int show_destinations();

int search_destination();

int add_destination();

int delete_destination();

int load_destinations();

int save_destinations();

int release_destination_memory();

int search_destination_by_id();

int search_destination_by_name();

int delete_destination_by_id();

int delete_destination_by_name();

int check_destination(Destination *destination);

int check_destination_name(char *name);

int check_destination_id(int id);

int set_destination_id(int id);