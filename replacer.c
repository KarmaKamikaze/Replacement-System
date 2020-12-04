#include "employee.h"
#include "positions.h"
#include "schedule.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int display_screen(const char *format, ...);

int main(int argc, char const *argv[]) {
  employee_s employees[MAX_POSITIONS];
  int menu_choice, num_of_employees, num_of_total_positions;
  char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH];
  num_of_total_positions = parse_positions(positions_str_arr);

  display_screen("", "Please choose the option below that you wish to execute.",
                 "", "1. Replacement of employee.", "2. Display all employees.",
                 "3. Add new employee.", "4. Edit employee.",
                 "5. Remove employee.", "6. Add new position.",
                 "7. Remove position.");

  system("clear");
  /* What isn't pretty in the code will have to be pretty elsewhere */
  printf("+--------------------------------------------------------------------"
         "----------+\n"
         "| ############################ REPLACEMENT SYSTEM "
         "############################ |\n"
         "+--------------------------------------------------------------------"
         "----------+\n"
         "| %-76s |\n| %-76s |\n" /* Prompt, then one empty line */
         "| %-76s |\n"
         "| %-76s |\n"
         "| %-76s |\n"
         "| %-76s |\n"
         "| %-76s |\n"
         "| %-76s |\n"
         "| %-76s |\n"
         "+--------------------------------------------------------------------"
         "----------+\n\n"
         "Choice: ",
         "Please choose the option below that you wish to execute.", " ",
         "1. Replacement of employee.", "2. Display all employees.",
         "3. Add new employee.", "4. Edit employee.", "5. Remove employee.",
         "6. Add new position.", "7. Remove position.");

  scanf(" %d", &menu_choice);

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

  schedule_function(employees, num_of_employees);

  return 0;
}

int display_screen(const char *format, ...) {
  va_list args;
  int i, menu_choice;

  system("clear");
  /* What isn't pretty in the code will have to be pretty elsewhere */
  printf("+--------------------------------------------------------------------"
         "----------+\n"
         "| ############################ REPLACEMENT SYSTEM "
         "############################ |\n"
         "+--------------------------------------------------------------------"
         "----------+\n");
  va_start(args, format);
  for (i = 0; i <= sizeof(format); i++) {
    printf("| %-76s |\n", va_arg(args, char *));
  }
  va_end(args);
  printf("+--------------------------------------------------------------------"
         "----------+\n\n"
         "Choice: ");
  scanf(" %d", &menu_choice);
  return menu_choice;
}
