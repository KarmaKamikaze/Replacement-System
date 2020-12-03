#include <stdio.h>
#include <stdlib.h>

#define MAX_EMPLOYEES 40
#define MAX_POSITIONS 100
#define MAX_LINE_LENGTH 500
#define MAX_STRING_LENGTH 100
#define MAX_PHONE 9

typedef struct employee_s {
  char name[MAX_STRING_LENGTH];
  int youth_worker;         /* Boolean */
  int weekday_availability; /* Boolean */
  char phone_number[MAX_PHONE];
  int number_of_positions; /* Used internally */
  char positions[MAX_POSITIONS][MAX_STRING_LENGTH];
} employee_s;

/* Function prototypes */
int parse_employee_data(employee_s employees[]);
int count_elements(FILE *fp);
void print_employee(const employee_s employees[], int num_of_employees);


void store_positions(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH], int num_of_total_positions);
int parse_positions(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH]);
void new_position(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH], int *num_of_total_positions);

