#include <stdio.h>

#ifndef UTILITY_H
#define UTILITY_H

#ifndef MAX_STRING_LENGTH
#define MAX_STRING_LENGTH 200
#define MAX_LINE_LENGTH 500
#endif

/* Function prototypes */
int count_elements(FILE *fp);
void capitalize_string(char *str);
void str_mem_alloc_check(char *dynamic_array);
void file_open_check(FILE *file_pointer);
void display_screen(char print_list[][MAX_STRING_LENGTH], int size);
void wait(unsigned int time);

#endif
