#include <stdio.h>
#include <string.h>
#include "schedule.h"

#define MAX_POSITIONS 100
#define MAX_LINE_LENGTH 500
#define MAX_STRING_LENGTH 100
#define MAX_PHONE 9

typedef struct employee_s {
  char name[MAX_STRING_LENGTH];
  int youth_worker;         /* Boolean */
  int weekday_availability; /* Boolean */
  char phone_numbers[MAX_PHONE];
  int number_of_positions; /* Used internally */
  char positions[MAX_POSITIONS][MAX_STRING_LENGTH];
} employee_s;

/* Function prototypes */
int parse_employee_data(employee_s employee[]);
int count_elements(FILE *fp);
void print_employee(const employee_s employee[], int num_of_employees);

int main(int argc, char const *argv[]) {
  employee_s employee[MAX_POSITIONS];
  int num_of_employees;

  print_something();

  num_of_employees = parse_employee_data(employee);
  print_employee(employee, num_of_employees);

  return 0;
}

/**
 * @brief This function parses the information, stored in a file, into the
 * employee struct array, which it then returns together with the amount of
 * employees.
 *
 * @param employee Output parameter. An employee struct array.
 * @return int The amount of employees parsed from file to array.
 */
int parse_employee_data(employee_s employee[]) {
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
    sscanf(input_string, "%[^,],%d,%d,%8[^,],%d,%[^\n]", employee[i].name,
           &employee[i].youth_worker, &employee[i].weekday_availability,
           employee[i].phone_numbers, &employee[i].number_of_positions,
           temp_positions);
    employee[i].name[MAX_STRING_LENGTH - 1] = '\0';
    employee[i].phone_numbers[MAX_PHONE - 1] = '\0';

    /* Get the first token */
    token = strtok(temp_positions, ",");
    /* Get remaining tokens */
    j = 0;
    while (token != NULL) {
      strcpy(employee[i].positions[j], token);
      token = strtok(NULL, ",");
      j++;
    }
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
 * @brief This function prints the employee struct array to show that it has
 * been filled correctly.
 *
 * @param employee An employee struct array witch is prefilled.
 * @param num_of_employees The number of employees in the struct array.
 */
void print_employee(const employee_s employee[], int num_of_employees) {
  int i, j;

  for (i = 0; i < num_of_employees; i++) {
    printf("%s %d %d %s %d", employee[i].name, employee[i].youth_worker,
           employee[i].weekday_availability, employee[i].phone_numbers,
           employee[i].number_of_positions);
    j = 0;
    while (employee[i].positions[j][0] != '\0') {
      printf(" %s", employee[i].positions[j]);
      j++;
    }
    printf("\n");
  }
}
