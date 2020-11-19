#include <stdio.h>

#define MAX_POSITIONS 20

typedef enum titles {
  closing_responsible,
  employee,
  head_manager,
  manager,
  opening_responsible,
  shift_supervisor
} titles;

typedef struct employee_s {
  int youth_worker;        /* Boolean */
  int number_of_positions; /* Used internally */
  int positions[MAX_POSITIONS];
} employee_s;

void print_employee(employee_s employee);

int main(int argc, char const *argv[]) {
  employee_s employee;
  employee.youth_worker = 1;
  employee.number_of_positions = 1;
  employee.positions[0] = manager;
  employee.positions[1] = shift_supervisor;

  print_employee(employee);

  return 0;
}

void print_employee(employee_s employee) {
  int i;
  int size = employee.number_of_positions;

  printf("Under 18? %s\nTitles:", employee.youth_worker ? "Yes." : "No.");
  for (i = 0; i <= size; i++) {
    printf(" %d", employee.positions[i]);
  }
  printf("\n");
}