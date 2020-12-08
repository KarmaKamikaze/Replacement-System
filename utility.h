#include <stdio.h>

#ifndef UTILITY_H
#define UTILITY_H

/* Function prototypes */
int count_elements(FILE *fp);
void capitalize_string(char *str);
void str_mem_alloc_check(char *dynamic_array);
void file_open_check(FILE *file_pointer);
int display_screen(char *print_list[], int size);

#endif
