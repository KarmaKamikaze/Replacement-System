#include <stdio.h>
#include <string.h>

#define MAX_POSITIONS 100
#define MAX_STRING_LENGTH 100
#define MAX_PHONE 9

/* typedef enum titles {
  closing_responsible,
  employee,
  head_manager,
  manager,
  opening_responsible,
  shift_supervisor
} titles; */

typedef struct employee_s {
  char name[MAX_STRING_LENGTH];
  int youth_worker; /* Boolean */
  int weekday_availability;
  char phone_numbers[MAX_PHONE];
  int number_of_positions; /* Used internally */
  char positions[MAX_POSITIONS][MAX_STRING_LENGTH];
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
  char temp_positions[MAX_POSITIONS], input_string[500];
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

  /* Makes sure we ALWAYS read from the start of our file. */
  fseek(fp, 0, SEEK_SET);  
  for (i = 0; i <= count_elements(fp); i++) {
    fgets(input_string, 500, fp);      
    sscanf(input_string, "%[^,],%d,%d,%[^,],%d,%s\n", employee[i].name,
           &employee[i].youth_worker, &employee[i].weekday_availability,
           employee[i].phone_numbers, &employee[i].number_of_positions,
           temp_positions);

    employee[i].positions = strtok(temp_positions, ',');
    /* WHILE LOOP TIL STRKTOK, SØGE EFTER ALLE CHARACTERS I TXT */

  }

  fclose(fp);
}

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

void print_employee(employee_s employee[]) {
  int i;
  int size = employee[0].number_of_positions;

  printf("Under 18? %s\nTitles:", employee[0].youth_worker ? "Yes." : "No.");
  for (i = 0; i <= size; i++) {
    /* printf(" %s", employee[i].positions[0]); */
  }
  printf("\n");
}
