#include "employee.h"
#include "schedule.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Function prototypes */
int parse_employee_data(employee_s employees[]);
int count_elements(FILE *fp);
void store_employee_data(const employee_s employees[], int num_of_employees);
void print_employee(const employee_s employees[], int num_of_employees);

void store_positions(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                     int num_of_total_positions);
int parse_positions(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH]);
void new_position(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                  int *num_of_total_positions);
void delete_position(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                     int *num_of_total_positions);

int main(int argc, char const *argv[]) {
  employee_s employees[MAX_POSITIONS];
  int num_of_employees, num_of_total_positions;
  char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH];
  num_of_total_positions = parse_positions(positions_str_arr);

  new_position(positions_str_arr, &num_of_total_positions);
  delete_position(positions_str_arr, &num_of_total_positions);

  store_positions(positions_str_arr, num_of_total_positions);

  num_of_employees = parse_employee_data(employees);
  print_employee(employees, num_of_employees);

  add_new_employee(employees, &num_of_employees, positions_str_arr,
                   num_of_total_positions);

  edit_employee(employees, num_of_employees, positions_str_arr,
                num_of_total_positions);

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

int parse_positions(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH]) {
  FILE *fp;
  int i, num_of_total_positions;
  char input_string[MAX_STRING_LENGTH];

  do {
    fp = fopen("positions.txt", "r");
    if (fp == NULL) {
      printf("File positions.txt was not found. Creating new file.");
      fp = fopen("positions.txt", "w");
      fclose(fp);
    }
  } while (fp == NULL);

  printf("Elements counted: %d\n", num_of_total_positions = count_elements(fp));

  fseek(fp, 0, SEEK_SET);

  for (i = 0; i < num_of_total_positions; i++) {
    fgets(input_string, MAX_STRING_LENGTH, fp);
    strcpy(positions_str_arr[i], input_string);
  }

  fclose(fp);

  return num_of_total_positions;
}

void store_positions(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                     int num_of_total_positions) {
  FILE *fp;
  int i;

  fp = fopen("positions.txt", "w");

  for (i = 0; i < num_of_total_positions; i++)
    fprintf(fp, "%s", positions_str_arr[i]);

  /* Check if last positions final character is a newline. If not, print a
   * newline. */
  if (positions_str_arr[num_of_total_positions - 1]
                       [strlen(positions_str_arr[num_of_total_positions - 1]) -
                        1] != '\n')
    fprintf(fp, "\n");
  fclose(fp);
}

void new_position(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                  int *num_of_total_positions) {
  char temp_string[MAX_STRING_LENGTH], throwaway_string[MAX_STRING_LENGTH];
  bool duplicate_check;
  int i;

  printf("ENTER NEW POSITION: ");
  scanf("%[^\n]", temp_string);
  fgets(throwaway_string, MAX_STRING_LENGTH, stdin);
  strcpy(temp_string, capitalize_string(temp_string));

  for (i = 0; i < *num_of_total_positions; i++) {
    if (!strncmp(positions_str_arr[i], temp_string, strlen(temp_string) - 2)) {
      duplicate_check = true;
      printf("THIS POSITION ALREADY EXISTS!\n");
      break;
    }
  }
  if (duplicate_check == false) {
    strcpy(positions_str_arr[*num_of_total_positions], temp_string);
    printf("POSITION %s HAS BEEN ADDED\n",
           positions_str_arr[*num_of_total_positions]);
    (*num_of_total_positions)++;
  }
}

void delete_position(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                     int *num_of_total_positions) {
  int position_value;
  char throwaway_string[MAX_STRING_LENGTH];
  int i;

  printf("\nEXISTING POSITIONS:\n");
  for (i = 0; i < *num_of_total_positions; i++)
    printf("%d = %s", i + 1, positions_str_arr[i]);

  printf("\nENTER DIGIT CORRESPONDING TO POSITION TO DELETE: ");
  scanf("%d", &position_value);
  fgets(throwaway_string, MAX_STRING_LENGTH, stdin);

  if (position_value >= 1 && position_value <= *num_of_total_positions) {
    for (i = position_value - 1; i < *num_of_total_positions; i++) {
      strcpy(positions_str_arr[i], positions_str_arr[i + 1]);
    }
    (*num_of_total_positions)--;
  } else {
    printf("THE CHOSEN POSITION DOES NOT EXIST!\n");
  }
}
