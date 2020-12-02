#include "replacer.h"

void add_new_employee(employee_s employees[], int *num_of_employees, char positions_str_arr[MAX_POSITIONS][20]);
void scan_name_add_employee(employee_s employees[], int current_employee);
void scan_phone_number(employee_s employees[], int current_employee);
void scan_youth_worker_or_availability(employee_s employees[], int current_employee, char *string_youth_or_availability);
void scan_number_of_positions(employee_s employees[], int current_employee);
void scan_positions(employee_s employees[], int current_employee, char positions_str_arr[MAX_POSITIONS][20]);
void print_employee_after_adding(employee_s employees[], int current_employee, char positions_str_arr[MAX_POSITIONS][20]);

void scan_name_edit_employee(employee_s employees[], int *num_of_employees, int *current_employee);
void capitalize_string(char *str);
