#include "employee.h"
#include "positions.h"
#include "schedule.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>

void menu_options(int user_choice, employee_s employees[],
                  char positions_str_arr[][MAX_STRING_LENGTH],
                  int *num_of_employees, int *num_of_total_positions);

int main(int argc, char const *argv[]) {
  employee_s employees[MAX_POSITIONS];
  int num_of_employees, num_of_total_positions;
  char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH];
  char *menu_prompts[MAX_STRING_LENGTH] = {
      "Please choose the option below that you wish to execute.",
      "",
      "1. Replacement of employee.",
      "2. Display all employees.",
      "3. Add new employee.",
      "4. Edit employee.",
      "5. Remove employee.",
      "6. Add new position.",
      "7. Remove position."};

  num_of_employees = parse_employee_data(employees);
  num_of_total_positions = parse_positions(positions_str_arr);

  menu_options(display_screen(menu_prompts), employees, positions_str_arr,
               &num_of_employees, &num_of_total_positions);

  store_employee_data(employees, num_of_employees);
  store_positions(positions_str_arr, num_of_total_positions);

  return 0;
}

void menu_options(int user_choice, employee_s employees[],
                  char positions_str_arr[][MAX_STRING_LENGTH],
                  int *num_of_employees, int *num_of_total_positions) {

  switch (user_choice) {
  case 1:
    schedule_function(employees, *num_of_employees);
    break;

  case 2:
    print_employees(employees, *num_of_employees);
    break;

  case 3:
    add_new_employee(employees, num_of_employees, positions_str_arr,
                     *num_of_total_positions);
    break;

  case 4:
    edit_employee(employees, *num_of_employees, positions_str_arr,
                  *num_of_total_positions);
    break;

  case 5:
    delete_employee(employees, num_of_employees);
    break;

  case 6:
    new_position(positions_str_arr, num_of_total_positions);
    break;

  case 7:
    delete_position(positions_str_arr, num_of_total_positions);
    break;

  default:
    perror("Error");
    exit(EXIT_FAILURE);
    break;
  }
}
