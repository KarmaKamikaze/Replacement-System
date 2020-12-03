#include "employee.h"
#include <stdio.h>
#include <string.h>
#include "schedule.h"

/* Function prototypes */
int parse_employee_data(employee_s employees[]);
int count_elements(FILE *fp);
void store_employee_data(const employee_s employees[], int num_of_employees);
void print_employee(const employee_s employees[], int num_of_employees);


int main(int argc, char const *argv[]) {
  employee_s employees[MAX_POSITIONS];
  int num_of_employees;
  char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH] = {
      "pos0", "pos1", "pos2", "pos3",
      "pos4", "pos5"}; /*This array of strings is a placeholder for an array
                          that is extracted from a csv-file*/

  num_of_employees = parse_employee_data(employees);
  print_employee(employees, num_of_employees);

  add_new_employee(employees, &num_of_employees, positions_str_arr);

  /*edit_employee(employees, num_of_employees, positions_str_arr);*/

  delete_employee(employees, &num_of_employees);

  schedule_function();

  return 0;
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
  char temp_positions[MAX_POSITIONS], input_string[MAX_LINE_LENGTH];
  char *token;
  /* Creates a new file if it does not exist.*/
  do {
    fp = fopen("employee.csv", "r");
    if (fp == NULL) {
      printf("File employee.csv was not found. Creating new file.");
      fp = fopen("employee.csv", "w");
      fclose(fp);
    }
  } while (fp == NULL);

  printf("Elements counted: %d\n", num_of_elements = count_elements(fp));

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
 * @brief This function counts the elements in an employee file.
 *
 * @param fp A FILE pointer to the employee file.
 * @return int The number of elements, e.g. the amount of employees stored in
 * the file.
 */
int count_elements(FILE *fp) {
  int elements = 0, ch = 0;
  /* Makes sure we ALWAYS read from the start of our file. */
  fseek(fp, 0, SEEK_SET);
  while (ch != EOF) {
    ch = fgetc(fp);
    if (ch == '\n') {
      elements++;
    }
  }
  return elements;
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
 * @param employees An employee struct array witch is prefilled.
 * @param num_of_employees The number of employees in the struct array.
 */
void print_employee(const employee_s employees[], int num_of_employees) {
  int i, j;

  for (i = 0; i < num_of_employees; i++) {
    printf("%s %d %d %s %d", employees[i].name, employees[i].youth_worker,
           employees[i].weekday_availability, employees[i].phone_number,
           employees[i].number_of_positions);
    j = 0;
    while (employees[i].positions[j][0] != '\0') {
      printf(" %s", employees[i].positions[j]);
      j++;
    }
    printf("\n");
  }
}
