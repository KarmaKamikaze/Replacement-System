#include "employee.h"

#ifndef POSITIONS_H
#define POSITIONS_H

/* Function prototypes */
int parse_positions(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH]);
void store_positions(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                     int num_of_total_positions);
void new_position(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                  int *num_of_total_positions);
void delete_position(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                     int *num_of_total_positions);

#endif
