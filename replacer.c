#include <stdio.h>
#include <string.h>

#define MAX_POSITIONS 100
#define MAX_STRING_LENGTH 100

/* typedef enum titles {
  closing_responsible,
  employee,
  head_manager,
  manager,
  opening_responsible,
  shift_supervisor
} titles; */

typedef struct employee_s {
  int youth_worker;        /* Boolean */
  int number_of_positions; /* Used internally */
  char positions[MAX_STRING_LENGTH][MAX_POSITIONS];
} employee_s;

void print_employee(employee_s employee[]);
void parse_employee_data(employee_s employee[]);
int count_elements(FILE *fp);

int main(int argc, char const *argv[]) {
  employee_s employee[MAX_POSITIONS];
  /* employee[0].youth_worker = 1;
  employee[0].number_of_positions = 1;
  strcpy(employee[0].positions[0][0], "manager");
  strcpy(employee[0].positions[1][0], "shift_supervisor"); */

  /* print_employee(employee); */

  parse_employee_data(employee);

  return 0;
}

void parse_employee_data(employee_s employee[]) {
  FILE *fp;
  int i; /* Counter */

  /* Creates a new file if it does not exist.*/
  do {
    fp = fopen("employee.txt", "r");
    if (fp == NULL) {
      printf("file employee.txt was not found, creating new file.");
      fp = fopen("employee.txt", "w");
      fclose(fp);
    }
  } while (fp == NULL);

  printf(" elements counted %d \n", count_elements(fp));

  for (i = 0; i <= count_elements(fp); i++) {
  }
  /* fscanf(fp, "%s, %s, %d"); */

  fclose(fp);
}

int count_elements(FILE *fp) {
  int elements = 0, ch = 0;
  /* Makes sure we ALWAYS read from the start of our file. */
  fseek(fp, 0, SEEK_SET);
  while (ch != EOF) {
    ch = fgetc(fp);
    if (ch == ',' || ch == '\n') {
      elements++;
    }
  }
  return elements;
}

void print_employee(employee_s employee[]) {
  int i;
  int size = employee[0].number_of_positions;

  printf("Under 18? %s\nTitles:", employee[0].youth_worker ? "Yes." : "No.");
  for (i = 0; i <= size; i++) {
    /* printf(" %s", employee[i].positions[0]); */
  }
  printf("\n");
}
