/**
 * @file replacer.c
 * @author Casper Bekhøi Roskær (croska19@student.aau.dk)
 *         Lasse Mølgaard Andersen (lman20@student.aau.dk)
 *         Mads Lildholdt Hansen (mlha20@student.aau.dk)
 *         Nicolai Hejlesen Jørgensen (njarge20@student.aau.dk)
 *         Sarmilan Tharmabalan (stharm20@student.aau.dk)
 *         Simon Peter Rasmussen (spra20@student.aau.dk)
 *         Tobias Lausten (tlaust18@student.aau.dk)
 *         Field of study: Software. Student group: A411
 * @brief This program finds a replacement for an absent employee, based on
 * rules and legislation.
 * @version 0.1.0
 * @date 2020-12-18
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "employee.h"
#include "positions.h"
#include "schedule.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>

void menu_options(int user_choice, employee_s employees[],
                  char positions_str_arr[][MAX_STRING_LENGTH],
                  int *num_of_employees, int *num_of_total_positions);

int main(void) {
  employee_s employees[MAX_POSITIONS];
  int num_of_employees, num_of_total_positions, menu_choice;
  char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH];
  char menu_prompts[][MAX_STRING_LENGTH] = {
      "Please choose the option below that you wish to execute. 0 to quit.",
      "",
      "1. Replacement of employee.",
      "2. Display all employees.",
      "3. Add new employee.",
      "4. Edit employee.",
      "5. Remove employee.",
      "6. Add new position.",
      "7. Remove position."};

  do {
    num_of_employees = parse_employee_data(employees);
    num_of_total_positions = parse_positions(positions_str_arr);

    display_screen(menu_prompts,
                   sizeof(menu_prompts) / sizeof(menu_prompts[0]) - 1);
    scanf(" %d", &menu_choice);
    fflush(stdin); /* Used to clear the input buffer */
    clear_screen();
    if (menu_choice != 0) {
      menu_options(menu_choice, employees, positions_str_arr, &num_of_employees,
                   &num_of_total_positions);
    }

    store_employee_data(employees, num_of_employees);
    store_positions(positions_str_arr, num_of_total_positions);
  } while (menu_choice != 0);

  return 0;
}

/**
 * @brief A selective control structure that takes a user input choice of which
 * action they wish to take when the program loads. The choice determins the
 * function that will be run in the switch.
 *
 * @param user_choice A choice made by the user, represented by an integer.
 * @param employees An array of employee structs, which will be passed through
 * to the underlying functions.
 * @param positions_str_arr A string array of the currently initialized
 * positions.
 * @param num_of_employees The number of existing employees.
 * @param num_of_total_positions The total number of initialized positions.
 */
void menu_options(int user_choice, employee_s employees[],
                  char positions_str_arr[][MAX_STRING_LENGTH],
                  int *num_of_employees, int *num_of_total_positions) {

  switch (user_choice) {
  case 1:
    find_replacement(employees, *num_of_employees, *num_of_total_positions);
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
