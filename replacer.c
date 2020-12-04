#include "positions.h"
#include "schedule.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
  employee_s employees[MAX_POSITIONS];
  int num_of_employees, num_of_total_positions;
  char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH];
  num_of_total_positions = parse_positions(positions_str_arr);

  new_position(positions_str_arr, &num_of_total_positions);
  delete_position(positions_str_arr, &num_of_total_positions);

  store_positions(positions_str_arr, num_of_total_positions);

  num_of_employees = parse_employee_data(employees);
  print_employees(employees, num_of_employees);

  add_new_employee(employees, &num_of_employees, positions_str_arr,
                   num_of_total_positions);

  edit_employee(employees, num_of_employees, positions_str_arr,
                num_of_total_positions);

  delete_employee(employees, &num_of_employees);

  schedule_function(/* employees, num_of_employees */);

  return 0;
}
