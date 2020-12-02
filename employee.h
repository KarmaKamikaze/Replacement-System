#include "replacer.h"

void add_new_employee(employee_s employees[], int *num_of_employees, char positions_str_arr[MAX_POSITIONS][20]);
void edit_employee(employee_s employees[], int num_of_employees, char positions_str_arr[MAX_POSITIONS][20]); 
void delete_employee(employee_s employees[], int *num_of_employees);

void scan_name(employee_s employees[], int current_employee);
void scan_phone_number(employee_s employees[], int current_employee);
void scan_youth_worker_or_availability(employee_s employees[], int current_employee, char *string_youth_or_availability);
void scan_number_of_positions(employee_s employees[], int current_employee);
void scan_positions(employee_s employees[], int current_employee, char positions_str_arr[MAX_POSITIONS][20]);
void print_employee_after_adding_or_editing(employee_s employees[], int current_employee, char positions_str_arr[MAX_POSITIONS][20], char *string_add_or_edit);

void scan_name_edit_or_delete_employee(employee_s employees[], int num_of_employees, int *current_employee, char *string_edit_or_delete);
void choose_employee_information_to_change(int *information_to_change);
int finished_editing_or_delete_prompt(char *string_edit_or_delete);

char *capitalize_string(char *str);
