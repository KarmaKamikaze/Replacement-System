#include "employee.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_new_employee(employee_s employees[], int *num_of_employees,
                      char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                      int num_of_total_positions);
void edit_employee(employee_s employees[], int num_of_employees,
                   char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                   int num_of_total_positions);
void choose_employee_information_to_change(int *information_to_change);
void delete_employee(employee_s employees[], int *num_of_employees);
void scan_name(employee_s employees[], int current_employee);
void scan_phone_number(employee_s employees[], int current_employee);
void scan_youth_worker_or_availability(employee_s employees[],
                                       int current_employee,
                                       char *string_youth_or_availability);
void scan_number_of_positions(employee_s employees[], int current_employee,
                              int num_of_total_positions);
void scan_positions(employee_s employees[], int current_employee,
                    char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                    int num_of_total_positions);
void print_employee_after_adding_or_editing(
    employee_s employees[], int current_employee,
    char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
    char *string_add_or_edit);
void scan_name_edit_or_delete_employee(employee_s employees[],
                                       int num_of_employees,
                                       int *current_employee,
                                       char *string_edit_or_delete);
int finished_editing_or_delete_prompt(char *string_edit_or_delete);
int confirmation_prompt(char *string_add_edit_or_delete);
int parse_employee_data(employee_s employees[]);
void store_employee_data(const employee_s employees[], int num_of_employees);
void print_employees(employee_s employees[], int num_of_employees);
void sort_employees(employee_s employees[], int num_of_employees);
int compare_employees(const void *a, const void *b);

/**
 * @brief This function adds a new employee to the employees array
 *
 * @param employees Output parameter. An array of structs containing data about
 * each employee.
 * @param num_of_employees Output parameter. The number of employees. Used as
 * the index specifier in the employees array. Is incremented if an employee is
 * added.
 * @param positions_str_arr A string array containing the existing positions
 * that we wish to store in a file.
 * @param num_of_total_positions The number of strings, containing positions, in
 * the position string array.
 */
void add_new_employee(employee_s employees[], int *num_of_employees,
                      char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                      int num_of_total_positions) {

  while (confirmation_prompt("add")) {
    /*Note that the index of the employees array is 1 smaller that the
     *num_of_employees, which means that the 10th employee has the index 9.
     *Therefore the new employee (employee 11) has the index equal to
     *num_of_employees (10)*/
    scan_name(employees, *num_of_employees);

    scan_phone_number(employees, *num_of_employees);

    scan_youth_worker_or_availability(employees, *num_of_employees, "youth");

    scan_youth_worker_or_availability(employees, *num_of_employees,
                                      "availability");

    scan_number_of_positions(employees, *num_of_employees,
                             num_of_total_positions);

    scan_positions(employees, *num_of_employees, positions_str_arr,
                   num_of_total_positions);

    print_employee_after_adding_or_editing(employees, *num_of_employees,
                                           positions_str_arr, "add");

    (*num_of_employees)++;
  }
}

/**
 * @brief This function edits an employee's information.
 *
 * @param employees Output parameter. An array of structs containing data about
 * each employee.
 * @param num_of_employees The number of employees. Used as the size of the
 * employees array.
 * @param positions_str_arr A string array containing the existing positions
 * that we wish to store in a file.
 * @param num_of_total_positions The number of strings, containing positions, in
 * the position string array.
 */
void edit_employee(employee_s employees[], int num_of_employees,
                   char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                   int num_of_total_positions) {
  int current_employee, information_to_change;

  while (confirmation_prompt("edit")) {
    scan_name_edit_or_delete_employee(employees, num_of_employees,
                                      &current_employee, "edit");
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
        scan_youth_worker_or_availability(employees, current_employee,
                                          "availability");
        break;
      case 4:
        scan_number_of_positions(employees, current_employee,
                                 num_of_total_positions);
        scan_positions(employees, current_employee, positions_str_arr,
                       num_of_total_positions);
        break;
      default:
        break;
      }
      /*This while-loop short-circuits*/
    } while (information_to_change != 5 &&
             !finished_editing_or_delete_prompt("edit"));
    print_employee_after_adding_or_editing(employees, current_employee,
                                           positions_str_arr, "edit");
  }
}

/**
 * @brief This function lets the user decide what information of the employee
 * they want to change.
 *
 * @param information_to_change Output parameter. An integer that corresponds to
 * the different information that can be changed.
 */
void choose_employee_information_to_change(int *information_to_change) {

  do {
    printf("\nENTER DIGITS FOR THE CORRESPONDING EMPLOYEE INFORMATION YOU WANT "
           "TO CHANGE:\n"
           "0 = NAME\n"
           "1 = PHONE NUMBER\n"
           "2 = YOUTH WORKER\n"
           "3 = WEEKDAY AVAILABILITY\n"
           "4 = POSITIONS\n"
           "5 = CHANGE NOTHING\n");
    scanf(" %d", information_to_change);
    fflush(stdin); /* Used to clear the input buffer */
    if (*information_to_change < 0 || *information_to_change > 5) {
      printf("INVALID INPUT! TRY AGAIN!\n");
      wait_time(3);
    }

    clear_screen();
  } while (*information_to_change < 0 || *information_to_change > 5);
}

/**
 * @brief This function lets the user delete an employee.
 *
 * @param employees Output parameter. An array of structs containing data about
 * each employee.
 * @param num_of_employees Output parameter. The number of employees. Used as
 * the size of the employees array. Is decremented if an employee is deleted.
 */
void delete_employee(employee_s employees[], int *num_of_employees) {
  int current_employee, i, j;

  while (confirmation_prompt("delete")) {
    scan_name_edit_or_delete_employee(employees, *num_of_employees,
                                      &current_employee, "delete");

    printf("\nEMPLOYEE SELECTED:\n");
    printf("NAME: %s\n", employees[current_employee].name);
    printf("PHONE NUMBER: %s\n", employees[current_employee].phone_number);
    printf("YOUTH WORKER: %s\n",
           employees[current_employee].youth_worker == 1 ? "YES" : "NO");
    printf("AVAILABLE ON WEEKDAYS FROM 8-16: %s\n",
           employees[current_employee].weekday_availability == 1 ? "YES"
                                                                 : "NO");
    printf("NUMBER OF POSITIONS: %d\n",
           employees[current_employee].number_of_positions);
    printf("CHOSEN POSITIONS:\n");
    for (i = 0; i < employees[current_employee].number_of_positions; i++)
      printf("[%s]\n", employees[current_employee].positions[i]);

    if (finished_editing_or_delete_prompt("delete")) {
      printf("\nEMPLOYEE %s WAS SUCCESFULLY DELETED!\n",
             employees[current_employee].name);
      /*This for-loop deletes an employee by overriding the information with the
       *next employee in the array. All the next employees are rearranged one
       *index lower*/
      for (i = current_employee; i < *num_of_employees; i++) {
        strcpy(employees[i].name, employees[i + 1].name);
        strcpy(employees[i].phone_number, employees[i + 1].phone_number);
        employees[i].youth_worker = employees[i + 1].youth_worker;
        employees[i].weekday_availability =
            employees[i + 1].weekday_availability;
        employees[i].number_of_positions = employees[i + 1].number_of_positions;
        for (j = 0; j < employees[i + 1].number_of_positions; j++)
          strcpy(employees[i].positions[j], employees[i + 1].positions[j]);
      }
      (*num_of_employees)--;
    } else {
      printf("EMPLOYEE %s WAS NOT DELETED\n", employees[current_employee].name);
    }
    wait_time(3);
    clear_screen();
  }
}

/**
 * @brief This function scans after a name. It is only used in the add_employees
 * function
 *
 * @param employees Output parameter. An array of structs containing data about
 * each employee.
 * @param current_employee Integer corresponding to the index of the employee
 * currently being added, edited or deleted.
 */
void scan_name(employee_s employees[], int current_employee) {

  printf("\nFULL NAME:\n");
  scanf(" %[a-zA-Z ]", employees[current_employee].name);
  fflush(stdin); /* Used to clear the input buffer */
  capitalize_string(employees[current_employee].name);
  clear_screen();
}

/**
 * @brief This function scans after a phone number. If the number is too long or
 * too short the do-while loop repeats
 *
 * @param employees Output parameter. An array of structs containing data about
 * each employee.
 * @param current_employee Integer corresponding to the index of the employee
 * currently being added, edited or deleted.
 */
void scan_phone_number(employee_s employees[], int current_employee) {

  do {
    printf("\nPHONE NUMBER:\n");
    scanf(" %[0-9]", employees[current_employee].phone_number);
    fflush(stdin); /* Used to clear the input buffer */
    if (strlen(employees[current_employee].phone_number) != 8) {
      printf("INVALID INPUT! TRY AGAIN!\n");
      wait_time(3);
    }
    clear_screen();
  } while (strlen(employees[current_employee].phone_number) != 8);
}

/**
 * @brief This function asks a yes/no question. Case insensitive. After the
 * do-while loop, the answer is converted into a boolean integer.
 *
 * @param employees Output parameter. An array of structs containing data about
 * each employee.
 * @param current_employee Integer corresponding to the index of the employee
 * currently being added, edited or deleted.
 * @param string_youth_or_availability A string which is inserted when the
 * function is called and this is used to determine which question is asked.
 */
void scan_youth_worker_or_availability(employee_s employees[],
                                       int current_employee,
                                       char *string_youth_or_availability) {
  char temp_string[MAX_STRING_LENGTH];

  do {
    printf("\n%s (YES/NO) ",
           !strcmp(string_youth_or_availability, "youth")
               ? "IS EMPLOYEE A YOUTH WORKER?"
               : "IS EMPLOYEE AVAILABLE ON WEEKDAYS FROM 8-16?");
    scanf(" %s", temp_string);
    fflush(stdin); /* Used to clear the input buffer */
    capitalize_string(temp_string);
    if (strcmp(temp_string, "YES") != 0 && strcmp(temp_string, "NO") != 0) {
      printf("\nINVALID INPUT! TRY AGAIN!\n");
      wait_time(3);
    }
    clear_screen();
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

/**
 * @brief This function scans after the number of positions the employee has and
 * only accepts values larger than 0 and less than num_of_total_positions
 *
 * @param employees Output parameter. An array of structs containing data about
 * each employee.
 * @param current_employee Integer corresponding to the index of the employee
 * currently being added, edited or deleted.
 * @param num_of_total_positions The number of strings, containing positions, in
 * the position string array.
 */
void scan_number_of_positions(employee_s employees[], int current_employee,
                              int num_of_total_positions) {

  if (num_of_total_positions != 0) {
    do {
      printf("\nNUMBER OF POSITIONS\n");
      scanf(" %d", &employees[current_employee].number_of_positions);
      fflush(stdin); /* Used to clear the input buffer */
      if (employees[current_employee].number_of_positions >
              num_of_total_positions ||
          employees[current_employee].number_of_positions <= 0) {
        printf("\nINVALID INPUT! TRY AGAIN!\n");
        wait_time(3);
      }
      clear_screen();
    } while (employees[current_employee].number_of_positions >
                 num_of_total_positions ||
             employees[current_employee].number_of_positions <= 0);
  }
}

/**
 * @brief This function prints the choosable positions and scans after them. The
 * positions and their corresponding integers are printed. Afterwards it scans
 * for the integers in a for-loop, performing scanf overflow. As soon as an
 * integer is too small or too large, the for loop breaks and the do-while loop
 * starts over.
 *
 * @param employees Output parameter. An array of structs containing data about
 * each employee.
 * @param current_employee Integer corresponding to the index of the employee
 * currently being added, edited or deleted.
 * @param positions_str_arr A string array containing the existing positions
 * that we wish to store in a file.
 * @param num_of_total_positions The number of strings, containing positions, in
 * the position string array.
 */
void scan_positions(employee_s employees[], int current_employee,
                    char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                    int num_of_total_positions) {
  int i, j, has_duplicates_bool, number_of_scanned_numbers;
  int temp_array[MAX_POSITIONS];

  if (num_of_total_positions != 0) {
    do {
      printf(
          "\nENTER DIGITS FOR THE CORRESPONDING POSITIONS: (FORMAT: x,y,z)\n");
      for (i = 0; i <= num_of_total_positions - 1; i++) {
        printf("%d = %s\n", i + 1, positions_str_arr[i]);
      }
      number_of_scanned_numbers = 0;
      for (i = 0; i < employees[current_employee].number_of_positions; i++) {
        number_of_scanned_numbers += scanf("%d,", &temp_array[i]);
        temp_array[i]--;
        /*scanf returns the number of items scanned. number_of_scanned_numbers
         * is used to check if all positions are scanned in the while-loop*/

        if (temp_array[i] < 0 || temp_array[i] > num_of_total_positions) {
          printf("INVALID INPUT! TRY AGAIN!\n");
          wait_time(3);
          break;
        }
      }
      fflush(stdin); /* Used to clear the input buffer */

      /*This for-loop checks if there are duplicate values in the array of
       *positions. The outer for-loop counts the value that is being checked
       *while the inner for-loop checks if the other values in the array is
       *equal to i (the value the outer for-loop counts) Note the for-loops
       *condition. The outer for-loop doesn't count the last value, since it has
       *been checked earlier iterations. The inner for-loop however checks the
       *last value, because the second to last value has not been checked with
       *the last value*/
      has_duplicates_bool = 0;
      if (employees[current_employee].number_of_positions != 1) {
        for (i = 0; i < employees[current_employee].number_of_positions - 1;
             i++) {
          for (j = i + 1;
               j <= employees[current_employee].number_of_positions - 1; j++) {
            if (temp_array[i] == temp_array[j]) {
              has_duplicates_bool = 1;
              break;
            }
          }
          if (has_duplicates_bool) {
            printf("INVALID INPUT! TRY AGAIN!\n");
            wait_time(3);
            break;
          }
        }
      }
      clear_screen();
    } while (temp_array[i - 1] < 0 ||
             temp_array[i - 1] > num_of_total_positions ||
             number_of_scanned_numbers !=
                 employees[current_employee].number_of_positions ||
             has_duplicates_bool);
    /*Here 1 is subtracted from i, since the for-loop increments i before it
     * breaks, which means i is too large*/

    for (i = 0; i < employees[current_employee].number_of_positions; i++)
      strcpy(employees[current_employee].positions[i],
             positions_str_arr[temp_array[i]]);
  } else {
    printf("\nNO AVAILABLE POSITIONS TO CHOOSE! ADD NEW POSITIONS!\n");
    wait_time(3);
    clear_screen();
  }
}

/**
 * @brief This function prints the employee.
 *
 * @param employees An array of structs containing data about each employee.
 * @param current_employee Integer corresponding to the index of the employee
 * currently being added, edited or deleted.
 * @param positions_str_arr A string array containing the existing positions
 * that we wish to store in a file.
 * @param string_add_or_edit A string which is inserted when the function is
 * called. Used to determine which function has been executed through a printf
 * function.
 */
void print_employee_after_adding_or_editing(
    employee_s employees[], int current_employee,
    char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
    char *string_add_or_edit) {
  int j;

  printf("\nEMPLOYEE %s WAS SUCCESFULLY %s.\n",
         employees[current_employee].name,
         !strcmp(string_add_or_edit, "add") ? "ADDED" : "EDITED");
  printf("PHONE NUMBER: %s.\n", employees[current_employee].phone_number);
  printf("YOUTH WORKER: %s.\n",
         employees[current_employee].youth_worker == 1 ? "YES" : "NO");
  printf("AVAILABLE ON WEEKDAYS FROM 8-16: %s.\n",
         employees[current_employee].weekday_availability == 1 ? "YES" : "NO");
  printf("NUMBER OF POSITIONS: %d.\n",
         employees[current_employee].number_of_positions);
  printf("CHOSEN POSITIONS:\n");
  for (j = 0; j < employees[current_employee].number_of_positions; j++)
    printf("%s\n", employees[current_employee].positions[j]);
  wait_time(6);
  clear_screen();
}

/**
 * @brief This function scans after a name and checks if the name is in the
 * employees array.
 *
 * @param employees An array of structs containing data about each employee.
 * @param num_of_employees The number of employees. Used as the size of the
 * employees array.
 * @param current_employee Output parameter. Integer corresponding to the index
 * of the employee currently being added, edited or deleted.
 * @param string_edit_or_delete A string which is inserted when the function is
 * called. Used to determine which function to execute through a printf
 * function.
 */
void scan_name_edit_or_delete_employee(employee_s employees[],
                                       int num_of_employees,
                                       int *current_employee,
                                       char *string_edit_or_delete) {
  int i, found_employee_bool;
  char temp_name_string[MAX_STRING_LENGTH];

  do {
    printf("\nEMPLOYEE TO %s (FULL NAME)\n",
           !strcmp(string_edit_or_delete, "edit") ? "EDIT" : "DELETE");
    scanf(" %[a-zA-Z ]", temp_name_string);
    fflush(stdin); /* Used to clear the input buffer */
    capitalize_string(temp_name_string);
    found_employee_bool = 0;
    for (i = 0; i <= num_of_employees; i++) {
      if (!strcmp(temp_name_string, employees[i].name)) {
        found_employee_bool = 1;
        break;
      }
    }
    if (!found_employee_bool) {
      printf("EMPLOYEE NOT FOUND! TRY AGAIN!\n");
      wait_time(3);
    }
  } while (!found_employee_bool);
  *current_employee = i;
  clear_screen();
}

/**
 * @brief This function is used as a prompt and scans after a yes/no answer.
 *
 * @param string_edit_or_delete Used to determine which function has been
 * executed through a printf function.
 * @return A boolean integer.
 */
int finished_editing_or_delete_prompt(char *string_edit_or_delete) {
  char temp_string[4];

  do {
    printf("\n%s (YES/NO) ", !strcmp(string_edit_or_delete, "edit")
                                 ? "FINISHED EDITING EMPLOYEE?"
                                 : "ARE YOU SURE YOU WANT TO DELETE EMPLOYEE?");
    scanf(" %s", temp_string);
    fflush(stdin); /* Used to clear the input buffer */
    capitalize_string(temp_string);
    if (strcmp(temp_string, "YES") != 0 && strcmp(temp_string, "NO") != 0) {
      printf("INVALID INPUT! TRY AGAIN!\n");
      wait_time(3);
    }

  } while (strcmp(temp_string, "YES") != 0 && strcmp(temp_string, "NO") != 0);
  clear_screen();
  return !strcmp(temp_string, "YES");
}

/**
 * @brief This function is used as a prompt and scans after a yes/no answer.
 *
 * @param string_add_edit_or_delete Used to determine which function has been
 * executed through a printf function.
 * @return A boolean integer
 */
int confirmation_prompt(char *string_add_edit_or_delete) {
  char temp_yes_no_string[MAX_STRING_LENGTH];

  do {
    printf("\n%s AN EMPLOYEE? (YES/NO) ",
           !strcmp(string_add_edit_or_delete, "add")    ? "ADD"
           : !strcmp(string_add_edit_or_delete, "edit") ? "EDIT"
                                                        : "DELETE");
    scanf(" %s", temp_yes_no_string);
    fflush(stdin); /* Used to clear the input buffer */
    capitalize_string(temp_yes_no_string);
    if (strcmp(temp_yes_no_string, "YES") != 0 &&
        strcmp(temp_yes_no_string, "NO") != 0) {
      printf("INVALID INPUT! TRY AGAIN!\n");
      wait_time(3);
    }
    clear_screen();
  } while (strcmp(temp_yes_no_string, "YES") != 0 &&
           strcmp(temp_yes_no_string, "NO") != 0);

  return !strcmp(temp_yes_no_string, "YES");
}

/**
 * @brief This function parses the information, stored in a file, into the
 * employee struct array, which it then returns together with the amount of
 * employees.
 *
 * @param employees Output parameter. An employee struct array.
 * @return int The amount of employees parsed from file to array.
 */
int parse_employee_data(employee_s employees[]) {
  FILE *fp;
  int i, j; /* Counters */
  int num_of_elements;
  char temp_positions[MAX_STRING_LENGTH], input_string[MAX_LINE_LENGTH];
  char *token;
  /* Creates a new file if it does not exist.*/
  do {
    fp = fopen("employee.csv", "r");
    if (fp == NULL) {
      printf("File positions.txt was not found. Creating new file.\n");
      wait_time(3);
      clear_screen();
      fp = fopen("employee.csv", "w");
      fclose(fp);
    }
  } while (fp == NULL);

  num_of_elements = count_elements(fp);

  /* Makes sure we ALWAYS read from the start of our file. */
  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < num_of_elements; i++) {
    fgets(input_string, MAX_LINE_LENGTH, fp);
    sscanf(input_string, "%[^,],%d,%d,%8[^,],%*d,%[^\n]", employees[i].name,
           &employees[i].youth_worker, &employees[i].weekday_availability,
           employees[i].phone_number, temp_positions);
    employees[i].name[MAX_STRING_LENGTH - 1] = '\0';
    employees[i].phone_number[MAX_PHONE - 1] = '\0';

    /* Get the first token */
    token = strtok(temp_positions, ",");
    /* Get remaining tokens */
    j = 0;
    while (token != NULL) {
      strcpy(employees[i].positions[j], token);
      token = strtok(NULL, ",");
      j++;
    }
    employees[i].number_of_positions = j;
  }
  fclose(fp);
  return num_of_elements;
}

/**
 * @brief This function stored the array of employee structs to a file, after
 * the program has completed all other tasks. This is done to ensure that all
 * changes are stored for the next time the program is launched.
 *
 * @param employees An array of employee structs containing the information
 * about each worker.
 * @param num_of_employees The number of current employees in the system.
 */
void store_employee_data(const employee_s employees[], int num_of_employees) {
  FILE *fp;
  int i, j; /* Counters */

  fp = fopen("employee.csv", "w");

  for (i = 0; i < num_of_employees; i++) {
    fprintf(fp, "%s,%d,%d,%s,%d", employees[i].name, employees[i].youth_worker,
            employees[i].weekday_availability, employees[i].phone_number,
            employees[i].number_of_positions);
    for (j = 0; j < employees[i].number_of_positions; j++) {
      fprintf(fp, ",%s", employees[i].positions[j]);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}

/**
 * @brief This function prints the employee struct array to show that it has
 * been filled correctly.
 *
 * @param employees An employee struct array which is prefilled.
 * @param num_of_employees The number of employees in the struct array.
 */
void print_employees(employee_s employees[], int num_of_employees) {
  char c;
  int i, j;

  sort_employees(employees, num_of_employees);
  for (i = 0; i < num_of_employees; i++) {
    printf("\n%s | %s | %s | %s\n", employees[i].name,
           employees[i].youth_worker ? "YOUTH WORKER" : "NOT YOUTH WORKER",
           employees[i].weekday_availability
               ? "AVAILABLE ON WEEKDAYS FROM 8-16"
               : "NOT AVAILABLE ON WEEKDAYS FROM 8-16",
           employees[i].phone_number);
    j = 0;
    while (employees[i].positions[j][0] != '\0') {
      printf(" %s\n", employees[i].positions[j]);
      j++;
    }
  }
  printf("\n\nPRESS ENTER TO CONTINUE");
  while ((c = getchar()) != '\n' && c != EOF)
    continue;
  getchar();
  clear_screen();
}

/**
 * @brief This function sorts employees using qsort
 * @param employees An employee struct array which is prefilled.
 * @param num_of_employees The number of employees in the struct array.
 */
void sort_employees(employee_s employees[], int num_of_employees) {
  qsort(employees, num_of_employees, sizeof(employee_s), compare_employees);
}

/**
 * @brief This is the comparison function used in qsort that compares employees
 * based on their name
 * @param a element 1 for comparison
 * @param b element 2 for comparison
 */
int compare_employees(const void *a, const void *b) {
  employee_s *p1 = (employee_s *)a;
  employee_s *p2 = (employee_s *)b;

  return strcmp(p1->name, p2->name);
}
