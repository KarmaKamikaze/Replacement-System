#include <stdio.h>
#include <ctype.h>
#include <string.h>
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
  char name[100];
  char phone_number[9];
  int youth_worker; /* Boolean */
  int weekday_availability; /* Boolean */
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


  /*print_employee(employees[0]);*/

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

/*Denne funktion tilføjer en ny employee til arrayet employees[]. */
void add_new_employee(employee_s employees[], int *num_of_employee) {
  char positions_str_arr[TOTAL_POSITIONS][20] = {"pos0", "pos1", "pos2", "pos3", "pos4", "pos5"};
  char temp_string[10];
  int i;
  printf("FULL NAME: ");
  scanf(" %[a-zA-Z ]", employees[*num_of_employee+1].name);
  
  /*Denne do-while scanner efter 8-cifret telefonnummer. Hvis det er for langt eller for kort kører løkken igen*/
  do {
    printf("PHONE NUMBER: ");
    scanf(" %[0-9]", employees[*num_of_employee+1].phone_number);
    if (strlen(employees[*num_of_employee+1].phone_number) != 8)
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (strlen(employees[*num_of_employee+1].phone_number) != 8);
  
  /*De næste 2 do-whiles kan laves om til én funktion der kaldes to gange. Vi skal blive enige om hvordan vi laver funktioner*/
  /*These 2 next do-while loops asks a yes/no question and only accepts words that start with y or n. Case insensitive.
  After the loops, the answer is converted into a boolean integer*/
  do {
    printf("IS EMPLOYEE A YOUTH WORKER? (YES/NO) ");
    scanf(" %[YESNOyesno]", temp_string);
    if (toupper(temp_string[0]) != 'Y' && toupper(temp_string[0]) != 'N')
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (toupper(temp_string[0]) != 'Y' && toupper(temp_string[0]) != 'N');
  
  if (toupper(temp_string[0]) == 'Y'){
    employees[*num_of_employee+1].youth_worker = 1;}
  else {
    employees[*num_of_employee+1].youth_worker = 0;}

  do {
    printf("IS EMPLOYEE AVAILABLE ON WEEKDAYS FROM 8-16? (YES/NO) ");
    scanf(" %[YESNOyesno]", temp_string);
    if (toupper(temp_string[0]) != 'Y' && toupper(temp_string[0]) != 'N')
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (toupper(temp_string[0]) != 'Y' && toupper(temp_string[0]) != 'N');
  
  if (toupper(temp_string[0]) == 'Y'){
    employees[*num_of_employee+1].weekday_availability = 1;}
  else {
    employees[*num_of_employee+1].weekday_availability = 0;}

  /*This do-while loop asks for the number of positions the employee has and only accepts values larger than 0 and less than TOTAL_POSITIONS */
  do {
    printf("NUMBER OF POSITIONS: ");
    scanf(" %d", &employees[*num_of_employee+1].number_of_positions);
    if (employees[*num_of_employee+1].number_of_positions > TOTAL_POSITIONS || employees[*num_of_employee+1].number_of_positions <= 0)
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (employees[*num_of_employee+1].number_of_positions > TOTAL_POSITIONS || employees[*num_of_employee+1].number_of_positions <= 0);

  /*This do-while loop prints the available positions and their corresponding integers. 
  Afterwards it scans for the integers in a for-loop, performing scanf overflow. As soon as an integer is too small or too large, 
  the for loop breaks and the while loop starts over*/
  do {
    printf("ENTER DIGITS FOR THE CORRESPONDING POSITIONS: (FORMAT: x,y,z)\n");
    for (i = 0; i <= TOTAL_POSITIONS-1; i++)
      printf("%s = %d\n", positions_str_arr+i, i);  /*Indeksering virker ikke helt her, så der bruges +i istedet*/
    for (i = 0; i < employees[*num_of_employee+1].number_of_positions; i++){
      scanf("%d,", &employees[*num_of_employee+1].positions[i]);
      if (employees[*num_of_employee+1].positions[i] < 0 || employees[*num_of_employee+1].positions[i] > TOTAL_POSITIONS){
        printf("INVALID INPUT! TRY AGAIN!\n"); 
        break;
      }
    }
    /*HER SKAL DER LAVES ET FOR LOOP ELLER NOGET SOM TJEKKER EFTER DUPLICATES. FX kan brugeren skrive 1,2,4,5,1. 
    Et for loop skal breake og få brugeren til at indtaste listen igen!*/

  } while (employees[*num_of_employee+1].positions[i-1] < 0 || employees[*num_of_employee+1].positions[i-1] > TOTAL_POSITIONS); 
  /*Her trækkes 1 fra i, siden for loopet incrementer inden den breakes hvilket betyder at i er for høj*/
  
  printf("\nCHOSEN POSITIONS:\n");
  for (i = 0; i < employees[*num_of_employee+1].number_of_positions; i++) 
    printf("%s\n", positions_str_arr[employees[*num_of_employee+1].positions[i]]);
  
  printf("\nEMPLOYEE %s WAS SUCCESFULLY ADDED.\n", employees[*num_of_employee+1].name);
  printf("PHONE NUMBER: %s\n", employees[*num_of_employee+1].phone_number);
  printf("YOUTH WORKER: %s\n", employees[*num_of_employee+1].youth_worker == 1 ? "YES" : "NO");
  printf("AVAILABLE ON WEEKDAYS FROM 8-16: %s\n", employees[*num_of_employee+1].weekday_availability == 1 ? "YES" : "NO");
  printf("NUMBER OF POSITIONS: %d\n", employees[*num_of_employee+1].number_of_positions);
  /*Denne blok kan skrives som en printf, men dette er mere læsbart*/

 /*Slet employee funktionen kan bruges her til at slette hvis man ikke er tilfreds med resultatet*/
  num_of_employee++;
}