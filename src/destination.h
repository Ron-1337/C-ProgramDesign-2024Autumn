#include <stdio.h>
#include <windows.h>

#include "define.h"
#include "utils.h"

#define DESTINATION_FILE "destinations.txt"

extern Destination *destinations_head;

int destination_management();

int show_destinations();

int search_destination();

int add_destination();

int delete_destination();

int load_destinations();

int save_destinations();