#include <stdio.h>

#ifndef UTILITY_H
#define UTILITY_H

#include "employee.h"
#include "schedule.h"

/* Function prototypes */
int count_elements(FILE *fp);
void capitalize_string(char *str);
void employee_mem_alloc_check(employee_s *dynamic_array);
void schedule_mem_alloc_check(schedule_s *dynamic_array);
void file_open_check(FILE *file_pointer);
void display_screen(char print_list[][MAX_STRING_LENGTH], int size);
void clear_screen();
void wait_time(unsigned int time);

#endif
