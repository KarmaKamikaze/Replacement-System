#include "employee.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX_EMPLOYEES 40
#define TOTAL_POSITIONS 6



int main(int argc, char const *argv[]) {
  employee_s employees[MAX_EMPLOYEES];
  int num_of_employees = 10; /*Denne variable skal bestemmes ud fra csv filen. Værdien er bare en placeholder pt.*/
  char positions_str_arr[TOTAL_POSITIONS][20] = {"pos0", "pos1", "pos2", "pos3", "pos4", "pos5"}; /*This array of strings is a placeholder for an array that is extracted from a csv-file*/
  
  add_new_employee(employees, &num_of_employees, positions_str_arr);

  return 0;
}
