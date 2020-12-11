#include "utility.h"
#include <stdbool.h>

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#define MAX_STRING_LENGTH 200
#define MAX_LINE_LENGTH 500
#define MAX_DISPLAY_ELEMENTS 40

#define MAX_EMPLOYEES 40
#define MAX_POSITIONS 100
#define MAX_PHONE 9

typedef struct employee_s {
  char name[MAX_STRING_LENGTH];
  int youth_worker;         /* Boolean */
  int weekday_availability; /* Boolean */
  char phone_number[MAX_PHONE];
  int number_of_positions; /* Used internally */
  char positions[MAX_POSITIONS][MAX_STRING_LENGTH];
  int points;        /*internal score system to rank possible replacements*/
  bool is_qualified; /*internal value to sort possible replacements*/
} employee_s;

/* Function prototypes */
void add_new_employee(employee_s employees[], int *num_of_employees,
                      char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                      int num_of_total_positions);
void edit_employee(employee_s employees[], int num_of_employees,
                   char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                   int num_of_total_positions);
void delete_employee(employee_s employees[], int *num_of_employees);
int parse_employee_data(employee_s employees[]);
void store_employee_data(const employee_s employees[], int num_of_employees);
void print_employees(employee_s employees[], int num_of_employees);

#endif
