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

/*void print_employee(employee_s employee);*/
void add_new_employee(employee_s employees[], int *num_of_employees);
void scan_name(employee_s employees[], int current_employee, char *throwaway_string);
void scan_phone_number(employee_s employees[], int current_employee, char *throwaway_string);
void scan_youth_worker_or_availability(employee_s employees[], int current_employee, char *throwaway_string, char *string_youth_or_availability);
void scan_number_of_positions(employee_s employees[], int current_employee, char *throwaway_string);
void scan_positions(employee_s employees[], int current_employee, char *throwaway_string , char positions_str_arr[TOTAL_POSITIONS][20]);
void print_employee_after_adding(employee_s employees[], int current_employee, char positions_str_arr[TOTAL_POSITIONS][20]);

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

/*
void print_employee(employee_s employee) {
  int i;
  int size = employee.number_of_positions;

  printf("Under 18? %d\nTitles:", employee.youth_worker);
  for (i = 0; i <= size; i++) {
    printf(" %d\n", employee.positions[i]);
  }
}
*/

/*Denne funktion tilføjer en ny employee til arrayet employees[]. */
void add_new_employee(employee_s employees[], int *num_of_employees) {
  char positions_str_arr[TOTAL_POSITIONS][20] = {"pos0", "pos1", "pos2", "pos3", "pos4", "pos5"};
  char throwaway_string[10]; /*throwaway_string is used to prevent scanf-overflow*/
  
  
  scan_name(employees, *num_of_employees+1, throwaway_string);

  scan_phone_number(employees, *num_of_employees+1, throwaway_string);

  scan_youth_worker_or_availability(employees, *num_of_employees+1, throwaway_string, "youth");
  
  scan_youth_worker_or_availability(employees, *num_of_employees+1, throwaway_string, "availability");
  
  scan_number_of_positions(employees, *num_of_employees+1, throwaway_string);
  
  scan_positions(employees, *num_of_employees+1, throwaway_string, positions_str_arr);
  
  print_employee_after_adding(employees, *num_of_employees+1, positions_str_arr);
  
   


 /*Slet employee funktionen kan bruges her til at slette hvis man ikke er tilfreds med resultatet*/
  *num_of_employees++;
}



/*This function scans after a name*/
void scan_name(employee_s employees[], int current_employee, char *throwaway_string){
  printf("FULL NAME: ");
  scanf(" %[a-zA-Z ]", employees[current_employee].name);
  gets(throwaway_string);
}

/*This function scans after a phone number. If the number is too lang or too short the do-while loop repeats*/
void scan_phone_number(employee_s employees[], int current_employee, char *throwaway_string){
  do {
    printf("PHONE NUMBER: ");
    scanf(" %[0-9]", employees[current_employee].phone_number);
    gets(throwaway_string);
    if (strlen(employees[current_employee].phone_number) != 8)
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (strlen(employees[current_employee].phone_number) != 8);
}

/*This function asks a yes/no question and only accepts words that start with y or n. Case insensitive.
 *After the loops, the answer is converted into a boolean integer.
 *A string is inserted when the function is called and this is used to determine which question is asked*/
void scan_youth_worker_or_availability(employee_s employees[], int current_employee, char *throwaway_string, char *string_youth_or_availability){
  char temp_string[10];
  do {
    printf("%s (YES/NO) ", !strcmp(string_youth_or_availability, "youth") ? "IS EMPLOYEE A YOUTH WORKER?" : "IS EMPLOYEE AVAILABLE ON WEEKDAYS FROM 8-16?");
    scanf(" %s", temp_string);
    gets(throwaway_string);
    if (toupper(temp_string[0]) != 'Y' && toupper(temp_string[0]) != 'N')
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (toupper(temp_string[0]) != 'Y' && toupper(temp_string[0]) != 'N');
  
  if (!strcmp(string_youth_or_availability, "youth")){
    if (toupper(temp_string[0]) == 'Y'){
      employees[current_employee].youth_worker = 1;}
    else {
      employees[current_employee].youth_worker = 0;}
  }

  if (!strcmp(string_youth_or_availability, "availability")){
    if (toupper(temp_string[0]) == 'Y'){
      employees[current_employee].weekday_availability = 1;}
    else {
      employees[current_employee].weekday_availability = 0;}
  }
}


/*This function scans after the number of positions the employee has and only accepts values larger than 0 and less than TOTAL_POSITIONS*/
void scan_number_of_positions(employee_s employees[], int current_employee, char *throwaway_string){
  do {
    printf("NUMBER OF POSITIONS: ");
    scanf(" %d", &employees[current_employee].number_of_positions);
    gets(throwaway_string);
    if (employees[current_employee].number_of_positions > TOTAL_POSITIONS || employees[current_employee].number_of_positions <= 0)
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (employees[current_employee].number_of_positions > TOTAL_POSITIONS || employees[current_employee].number_of_positions <= 0);
}


/*This function prints the choosable positions and scans after them. The positions and their corresponding integers are printed.
 *Afterwards it scans for the integers in a for-loop, performing scanf overflow. As soon as an integer is too small or too large, 
 *the for loop breaks and the while loop starts over*/
void scan_positions(employee_s employees[], int current_employee, char *throwaway_string , char positions_str_arr[TOTAL_POSITIONS][20]){
  int i, j, has_duplicates_bool, number_of_scanned_numbers;

  do {
    printf("ENTER DIGITS FOR THE CORRESPONDING POSITIONS: (FORMAT: x,y,z)\n");
    for (i = 0; i <= TOTAL_POSITIONS-1; i++)
      printf("%s = %d\n", positions_str_arr+i, i);  /*Indeksering virker ikke helt her, så der bruges +i istedet*/
    number_of_scanned_numbers = 0;
    for (i = 0; i < employees[current_employee].number_of_positions; i++){
      number_of_scanned_numbers += scanf("%d,", &employees[current_employee].positions[i]); 
      /*scanf returns the number of items scanned. number_of_scanned_numbers is used to check if all positions are scanned in the while-loop*/
      
      if (employees[current_employee].positions[i] < 0 || employees[current_employee].positions[i] > TOTAL_POSITIONS){
        printf("INVALID INPUT! TRY AGAIN!\n"); 
        break;
      }
    }
    gets(throwaway_string);

    /*This for-loop checks if there are duplicate values in the array of positions. The outer for-loop counts the value that is being checked
     *while the inner for-loop checks if the other values in the array is equal to i (the value the outer for-loop counts)
     *Note the for-loops condition. The outer for-loop doesn't count the last value, since it has been checked earlier iterations. 
     *The inner for-loop however checks the last value, because the second to last value has not been checked with the last value*/
    has_duplicates_bool = 0;
    for (i = 0; i < employees[current_employee].number_of_positions - 1; i++){
      for (j = i + 1; j <= employees[current_employee].number_of_positions - 1; j++){
        if (employees[current_employee].positions[i] == employees[current_employee].positions[j]){
          has_duplicates_bool = 1;
          break;
        }
      }
      if (has_duplicates_bool){
        printf("INVALID INPUT! TRY AGAIN!\n");
        break;
      }  
    }

  } while (employees[current_employee].positions[i-1] < 0 || employees[current_employee].positions[i-1] > TOTAL_POSITIONS || number_of_scanned_numbers != employees[current_employee].number_of_positions ||has_duplicates_bool); 
  /*Her trækkes 1 fra i, siden for loopet incrementer inden den breakes hvilket betyder at i er for høj*/
}


/*This function prints the employee. The printf functions can be written into one, but this is more readable*/
void print_employee_after_adding(employee_s employees[], int current_employee, char positions_str_arr[TOTAL_POSITIONS][20]){
  int i;
 
  printf("\nEMPLOYEE %s WAS SUCCESFULLY ADDED.\n", employees[current_employee].name);
  printf("PHONE NUMBER: %s\n", employees[current_employee].phone_number);
  printf("YOUTH WORKER: %s\n", employees[current_employee].youth_worker == 1 ? "YES" : "NO");
  printf("AVAILABLE ON WEEKDAYS FROM 8-16: %s\n", employees[current_employee].weekday_availability == 1 ? "YES" : "NO");
  printf("NUMBER OF POSITIONS: %d\n", employees[current_employee].number_of_positions);
  printf("CHOSEN POSITIONS: ");
  for (i = 0; i < employees[current_employee].number_of_positions; i++) 
    printf("[%s] ", positions_str_arr[employees[current_employee].positions[i]]);
  printf("\n");
}




