#include "replacer.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

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





void add_new_employee(employee_s employees[], int *num_of_employees,
                      char positions_str_arr[MAX_POSITIONS][20]) {

  /*Note that the index of the employees array is 1 smaller that the
   *num_of_employees, which means that the 10th employee has the index 9.
   *Therefore the new employee (employee 11) has the index equal to
   *num_of_employees (10)*/
  scan_name(employees, *num_of_employees);

  scan_phone_number(employees, *num_of_employees);

  scan_youth_worker_or_availability(employees, *num_of_employees, "youth");

  scan_youth_worker_or_availability(employees, *num_of_employees,
                                    "availability");

  scan_number_of_positions(employees, *num_of_employees);

  scan_positions(employees, *num_of_employees, positions_str_arr);

  print_employee_after_adding_or_editing(employees, *num_of_employees, positions_str_arr, "add");

  /*Slet employee funktionen kan bruges her til at slette hvis man ikke er
   * tilfreds med resultatet*/

  (*num_of_employees)++;
}

void edit_employee(employee_s employees[], int num_of_employees, char positions_str_arr[MAX_POSITIONS][20]) {
  int current_employee, information_to_change;

  scan_name_edit_or_delete_employee(employees, num_of_employees, &current_employee, "edit");
  do {
    choose_employee_information_to_change(&information_to_change);

    switch (information_to_change) {
    case 0:
      scan_name(employees, current_employee);
      break;
    case 1:
      scan_phone_number(employees, current_employee);
      break;
    case 2:
      scan_youth_worker_or_availability(employees, current_employee, "youth");
      break;
    case 3:
      scan_youth_worker_or_availability(employees, current_employee, "availability");
      break;
    case 4:
      scan_number_of_positions(employees, current_employee);
      scan_positions(employees, current_employee, positions_str_arr);
      break;
    default:
      break;
    }
          /*This while-loop short-circuits*/
  } while (information_to_change != 5 && !finished_editing_or_delete_prompt("edit"));
  print_employee_after_adding_or_editing(employees, current_employee, positions_str_arr, "edit");
}

void delete_employee(employee_s employees[], int *num_of_employees){
  int current_employee, i, j;

  scan_name_edit_or_delete_employee(employees, *num_of_employees, &current_employee, "delete");

  printf("EMPLOYEE SELECTED:\n");
  printf("NAME: %s\n", employees[current_employee].name);
  printf("PHONE NUMBER: %s\n", employees[current_employee].phone_number);
  printf("YOUTH WORKER: %s\n",
         employees[current_employee].youth_worker == 1 ? "YES" : "NO");
  printf("AVAILABLE ON WEEKDAYS FROM 8-16: %s\n",
         employees[current_employee].weekday_availability == 1 ? "YES" : "NO");
  printf("NUMBER OF POSITIONS: %d\n",
         employees[current_employee].number_of_positions);
  printf("CHOSEN POSITIONS: ");
  for (i = 0; i < employees[current_employee].number_of_positions; i++)
    printf("[%s] ", employees[current_employee].positions[i]);
  printf("\n");

  if (finished_editing_or_delete_prompt("delete")){
    printf("EMPLOYEE %s WAS SUCCESFULLY DELETED\n", employees[current_employee].name);
    /*This for-loop deletes an employee by overriding the information with the next employee in the array.
     *All the next employees are rearranged one index lower*/
    for (i = current_employee; i < *num_of_employees; i++){
      strcpy(employees[i].name, employees[i+1].name);
      strcpy(employees[i].phone_number, employees[i+1].phone_number);
      employees[i].youth_worker = employees[i+1].youth_worker;
      employees[i].weekday_availability = employees[i+1].weekday_availability;
      employees[i].number_of_positions = employees[i+1].number_of_positions;
      for (j = 0; j < employees[i+1].number_of_positions; j++)
        strcpy(employees[i].positions[j], employees[i+1].positions[j]);
    }
    (*num_of_employees)--;
  }
  else 
    printf("EMPLOYEE %s WAS NOT DELETED\n", employees[current_employee].name);
}

void scan_name(employee_s employees[], int current_employee) {
  char throwaway_string[50]; /*throwaway_string is used to prevent
                                scanf-overflow*/
  printf("FULL NAME: ");
  scanf(" %[a-zA-Z ]", employees[current_employee].name);
  fgets(throwaway_string, 50, stdin);
}

/*This function scans after a phone number. If the number is too long or too
 * short the do-while loop repeats*/
void scan_phone_number(employee_s employees[], int current_employee) {
  char throwaway_string[50];
  do {
    printf("PHONE NUMBER: ");
    scanf(" %[0-9]", employees[current_employee].phone_number);
    fgets(throwaway_string, 50, stdin);
    if (strlen(employees[current_employee].phone_number) != 8)
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (strlen(employees[current_employee].phone_number) != 8);
}

/*This function asks a yes/no question and only accepts words that start with y
 *or n. Case insensitive. After the loops, the answer is converted into a
 *boolean integer.
 *A string is inserted when the function is called and this is used to determine
 *which question is asked*/
void scan_youth_worker_or_availability(employee_s employees[],
                                       int current_employee,
                                       char *string_youth_or_availability) {
  char temp_string[10], throwaway_string[50];
  do {
    printf("%s (YES/NO) ",
           !strcmp(string_youth_or_availability, "youth")
               ? "IS EMPLOYEE A YOUTH WORKER?"
               : "IS EMPLOYEE AVAILABLE ON WEEKDAYS FROM 8-16?");
    scanf(" %s", temp_string);
    fgets(throwaway_string, 50, stdin);
    strcpy(temp_string, capitalize_string(temp_string));
    if (strcmp(temp_string, "YES") != 0 && strcmp(temp_string, "NO") != 0)
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (strcmp(temp_string, "YES") != 0 && strcmp(temp_string, "NO") != 0);

  if (!strcmp(string_youth_or_availability, "youth")) {
    if (!strcmp(temp_string, "YES")) {
      employees[current_employee].youth_worker = 1;
    } else {
      employees[current_employee].youth_worker = 0;
    }
  }

  if (!strcmp(string_youth_or_availability, "availability")) {
    if (!strcmp(temp_string, "YES")) {
      employees[current_employee].weekday_availability = 1;
    } else {
      employees[current_employee].weekday_availability = 0;
    }
  }
}

/*This function scans after the number of positions the employee has and only
 * accepts values larger than 0 and less than MAX_POSITIONS*/
void scan_number_of_positions(employee_s employees[], int current_employee) {
  char throwaway_string[50];
  do {
    printf("NUMBER OF POSITIONS: ");
    scanf(" %d", &employees[current_employee].number_of_positions);
    fgets(throwaway_string, 50, stdin);
    if (employees[current_employee].number_of_positions > MAX_POSITIONS ||  /*MAX_POSITIONS SKAL ÆNDRES TIL EN VARIABEL num_of_positions alle steder i filen*/
        employees[current_employee].number_of_positions <= 0)
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (employees[current_employee].number_of_positions > MAX_POSITIONS ||
           employees[current_employee].number_of_positions <= 0);
}

/*This function prints the choosable positions and scans after them. The
 *positions and their corresponding integers are printed. Afterwards it scans
 *for the integers in a for-loop, performing scanf overflow. As soon as an
 *integer is too small or too large, the for loop breaks and the while loop
 *starts over*/
void scan_positions(employee_s employees[], int current_employee,
                    char positions_str_arr[MAX_POSITIONS][20]) {
  int i, j, has_duplicates_bool, number_of_scanned_numbers;
  char throwaway_string[50];
  int temp_array[MAX_POSITIONS];

  do {
    printf("ENTER DIGITS FOR THE CORRESPONDING POSITIONS: (FORMAT: x,y,z)\n");
    for (i = 0; i <= MAX_POSITIONS - 1; i++)
      printf("%s = %d\n", positions_str_arr + i,
             i); /*Indeksering virker ikke helt her, så der bruges +i istedet*/
    number_of_scanned_numbers = 0;
    for (i = 0; i < employees[current_employee].number_of_positions; i++) {
      number_of_scanned_numbers += scanf("%d,", &temp_array[i]);
      /*scanf returns the number of items scanned. number_of_scanned_numbers is
       * used to check if all positions are scanned in the while-loop*/

      if (temp_array[i] < 0 || temp_array[i] > MAX_POSITIONS) {
        printf("INVALID INPUT! TRY AGAIN!\n");
        break;
      }
    }
    fgets(throwaway_string, 50, stdin);

    /*This for-loop checks if there are duplicate values in the array of
     *positions. The outer for-loop counts the value that is being checked while
     *the inner for-loop checks if the other values in the array is equal to i
     *(the value the outer for-loop counts) Note the for-loops condition. The
     *outer for-loop doesn't count the last value, since it has been checked
     *earlier iterations. The inner for-loop however checks the last value,
     *because the second to last value has not been checked with the last
     *value*/
    has_duplicates_bool = 0;
    if (employees[current_employee].number_of_positions != 1) {
      for (i = 0; i < employees[current_employee].number_of_positions - 1; i++) {
        for (j = i + 1; j <= employees[current_employee].number_of_positions - 1;
            j++) {
          if (temp_array[i] == temp_array[j]) {
            has_duplicates_bool = 1;
            break;
          }
        }
        if (has_duplicates_bool) {
          printf("INVALID INPUT! TRY AGAIN!\n");
          break;
        }
      }
    }
  } while (temp_array[i - 1] < 0 || temp_array[i - 1] > MAX_POSITIONS ||
           number_of_scanned_numbers !=
               employees[current_employee].number_of_positions ||
           has_duplicates_bool);
  /*Here 1 is subtracted from i, since the for-loop increments i before it
   * breaks, which means i is too large*/

  for (i = 0; i < employees[current_employee].number_of_positions; i++)
    strcpy(employees[current_employee].positions[i],
           positions_str_arr[temp_array[i]]);
}

/*This function prints the employee. The printf functions can be written into
 * one, but this is more readable*/
void print_employee_after_adding_or_editing(employee_s employees[], int current_employee,
                                 char positions_str_arr[MAX_POSITIONS][20], char *string_add_or_edit) /*Dog skal max_positions ikke ændres her*/ {
  int i;

  printf("\nEMPLOYEE %s WAS SUCCESFULLY %s.\n",
         employees[current_employee].name, !strcmp(string_add_or_edit, "add")
               ? "ADDED" : "EDITED");
  printf("PHONE NUMBER: %s\n", employees[current_employee].phone_number);
  printf("YOUTH WORKER: %s\n",
         employees[current_employee].youth_worker == 1 ? "YES" : "NO");
  printf("AVAILABLE ON WEEKDAYS FROM 8-16: %s\n",
         employees[current_employee].weekday_availability == 1 ? "YES" : "NO");
  printf("NUMBER OF POSITIONS: %d\n",
         employees[current_employee].number_of_positions);
  printf("CHOSEN POSITIONS: ");
  for (i = 0; i < employees[current_employee].number_of_positions; i++)
    printf("[%s] ", employees[current_employee].positions[i]);
  printf("\n");
}

void scan_name_edit_or_delete_employee(employee_s employees[], int num_of_employees,
                             int *current_employee, char *string_edit_or_delete) {

  int i, found_employee_bool;
  char temp_name_string[50], throwaway_string[50];
  do {
    printf("EMPLOYEE TO %s (FULL NAME): ", !strcmp(string_edit_or_delete, "edit") ? "EDIT" : "DELETE");
    scanf("%[a-zA-Z ]", temp_name_string);
    fgets(throwaway_string, 50, stdin);
    found_employee_bool = 0;
    for (i = 0; i <= num_of_employees; i++) {
      if (!strcmp(capitalize_string(temp_name_string), capitalize_string(employees[i].name))) {
        found_employee_bool = 1;
        break;
      }
    }
    if (!found_employee_bool) {
      printf("EMPLOYEE NOT FOUND! TRY AGAIN!\n");
    }
  } while (!found_employee_bool);
  *current_employee = i;
}

void choose_employee_information_to_change(int *information_to_change){
  do {
    printf("ENTER DIGITS FOR THE CORRESPONDING EMPLOYEE INFORMATION YOU WANT TO CHANGE:\n"
           "0 = NAME\n"
           "1 = PHONE NUMBER\n"
           "2 = YOUTH WORKER\n"
           "3 = WEEKDAY AVAILABILITY\n"
           "4 = POSITIONS\n"
           "5 = CHANGE NOTHING\n");

    scanf("%d", information_to_change);
      if (*information_to_change < 0 || *information_to_change > 5)
        printf("INVALID INPUT! TRY AGAIN!\n");
  } while (*information_to_change < 0 || *information_to_change > 5);
}


/*This function returns a boolean value*/
int finished_editing_or_delete_prompt(char *string_edit_or_delete){
  char temp_string[4];
  do {
    printf("%s (YES/NO) ", !strcmp(string_edit_or_delete, "edit") ? "FINISHED EDITING EMPLOYEE?" : "ARE YOU SURE YOU WANT TO DELETE EMPLOYEE?");
    scanf(" %s", temp_string);
    strcpy(temp_string, capitalize_string(temp_string));
    if (strcmp(temp_string, "YES") != 0 && strcmp(temp_string, "NO") != 0)
      printf("INVALID INPUT! TRY AGAIN!\n");
  } while (strcmp(temp_string, "YES") != 0 && strcmp(temp_string, "NO") != 0);  
  return !strcmp(temp_string, "YES");   
}


char *capitalize_string(char *str) {
  int i;
  char *capitalized_string = (char *)calloc(strlen(str) , sizeof(char));
  if (capitalized_string == NULL){
    printf("ERROR ALLOCATING MEMORY!\n");
    exit(EXIT_FAILURE);
  }
  
  for (i = 0; i < strlen(str); i++)
    capitalized_string[i] = toupper(str[i]);
  return capitalized_string;
}
