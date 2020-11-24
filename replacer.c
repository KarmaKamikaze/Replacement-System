#include <stdio.h>
#include <ctype.h>
#define MAX_EMPLOYEES 40
#define TOTAL_POSITIONS 6

typedef enum titles {
  closing_responsible,
  employee,
  head_manager,
  manager,
  opening_responsible,
  shift_supervisor
} titles;

typedef struct employee_s {
  char *name;
  char *phone_number;
  int youth_worker; /* Boolean */
  int number_of_positions;
  int positions[TOTAL_POSITIONS];
} employee_s;

void print_employee(employee_s employee);
void add_new_employee(employee_s employees[], int *num_of_employee);

int main(int argc, char const *argv[]) {
  employee_s employees[MAX_EMPLOYEES];
  int num_of_employees = 10; /*Denne variable skal bestemmes ud fra csv filen. Værdien er bare en placeholder pt.*/
  employees[0].youth_worker = 1;
  employees[0].number_of_positions = 2;
  employees[0].positions[0] = manager;
  employees[0].positions[1] = shift_supervisor;


  print_employee(employees[0]);

  add_new_employee(employees, &num_of_employees);

  return 0;
}

void print_employee(employee_s employee) {
  int i;
  int size = employee.number_of_positions;

  printf("Under 18? %d\nTitles:", employee.youth_worker);
  for (i = 0; i <= size; i++) {
    printf(" %d\n", employee.positions[i]);
  }
}

void add_new_employee(employee_s employees[], int *num_of_employee) {
  char *positions_str_arr[TOTAL_POSITIONS] = {"pos1", "pos2", "pos3", "pos4", "pos5", "pos6"};
  char *temp_string;
  int i;
  printf("test before scanf:%s",employees[*num_of_employee+1].name);
  printf("FULL NAME: ");
  scanf(" %s", employees[*num_of_employee+1].name);
  printf("test");
  do {
    printf("PHONE NUMBER: ");
    scanf("%[0-9]", employees[*num_of_employee+1].phone_number);
    if (sizeof(employees[*num_of_employee+1].phone_number) != 8)
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (sizeof(employees[*num_of_employee+1].phone_number) != 8);
  
  do {
    printf("IS EMPLOYEE A YOUTH WORKER? ");
    scanf("%[YESNOyesno]", temp_string);
    if (toupper(temp_string[0]) != 'Y' && toupper(temp_string[0]) != 'N')
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (toupper(temp_string[0]) != 'Y' && toupper(temp_string[0]) != 'N');
  
  if (toupper(temp_string[0]) == 'Y')
    employees[*num_of_employee+1].youth_worker = 1;
  else 
    employees[*num_of_employee+1].youth_worker = 0;

  do {
    printf("NUMBER OF POSITIONS: ");
    scanf("%d", &employees[*num_of_employee+1].number_of_positions);
    if (employees[*num_of_employee+1].number_of_positions > TOTAL_POSITIONS)
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (employees[*num_of_employee+1].number_of_positions > TOTAL_POSITIONS);

  do {
    printf("ENTER DIGITS FOR THE CORRESPONDING POSITIONS:\n");
    for (i = 0; i <= TOTAL_POSITIONS; i++)
      printf("%s = %d\n", positions_str_arr[i], i);
    for (i = 0; i <= employees[*num_of_employee+1].number_of_positions; i++){
      scanf("%d, ", &employees[*num_of_employee+1].positions[i]);
      if (employees[*num_of_employee+1].positions[i] < 0 || employees[*num_of_employee+1].positions[i] > TOTAL_POSITIONS){
        printf("INVALID INPUT! TRY AGAIN!\n"); 
        break;
      }
    }
  } while (employees[*num_of_employee+1].positions[i] < 0 || employees[*num_of_employee+1].positions[i] > TOTAL_POSITIONS);
  
  num_of_employee++;
}