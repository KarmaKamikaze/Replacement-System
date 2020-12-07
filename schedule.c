#include "schedule.h"
#include "employee.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void schedule_function(employee_s employees[], int num_of_employees);
void fill_schedule_with_data(schedule_s schedule[], FILE *schedule_fp,
                             int shifts);
void edit_schedule(schedule_s schedule[], FILE *schedule_fp,
                   int number_of_shifts, employee_s employees[],
                   int num_of_employees);
int check_if_employee_exists(employee_s employees[], int num_of_employees,
                             char scanned_employee_name[]);

void change_schedule_file(schedule_s schedule[], FILE *schedule_fp,
                          int number_of_shifts, int i, int j);

void schedule_function(employee_s employees[], int num_of_employees) {
  schedule_s schedule[DAYS_IN_SCHEDULE];
  int number_of_shifts;
  FILE *schedule_fp = fopen("schedule.csv", "r");
  if (schedule_fp == NULL) {
    perror("Error: ");
    exit(EXIT_FAILURE);
  }
  number_of_shifts = count_elements(schedule_fp);
  fill_schedule_with_data(schedule, schedule_fp, number_of_shifts);

  fclose(schedule_fp);

  edit_schedule(schedule, schedule_fp, number_of_shifts, employees,
                num_of_employees);
}

void fill_schedule_with_data(schedule_s schedule[], FILE *schedule_fp,
                             int number_of_shifts) {
  int i;
  char input_string[MAX_STRING_LENGTH];

  fseek(schedule_fp, 0, SEEK_SET);
  for (i = 0; i < number_of_shifts; i++) {
    fgets(input_string, MAX_STRING_LENGTH, schedule_fp);
    sscanf(input_string, "%[^,],%d,%d,%lf,%lf,%[^,],%[^,],%d\n",
           schedule[i].weekday, &schedule[i].day, &schedule[i].month,
           &schedule[i].shift_start, &schedule[i].shift_end,
           schedule[i].employee_name, schedule[i].shift_position,
           &schedule[i].youth_worker);
    schedule[i].weekday[MAX_WEEK_DAY_NAME - 1] = '\0';
    schedule[i].employee_name[MAX_STRING_LENGTH - 1] = '\0';
    schedule[i].shift_position[MAX_STRING_LENGTH - 1] = '\0';
  }
}

void edit_schedule(schedule_s schedule[], FILE *schedule_fp,
                   int number_of_shifts, employee_s employees[],
                   int num_of_employees) {
  int i, j = 0;
  int day, month;
  char name_of_absent_employee[MAX_STRING_LENGTH];

  printf("Enter day and month for sick person.");
  scanf(" %d/%d", &day, &month);

  for (i = 0; i < MAX_STRING_LENGTH; i++) {
    while (schedule[i].day == day && schedule[i].month == month) {

      printf("vagt nr. %d Medarbejder: %s\n", j, schedule[i].employee_name);
      j++;
      i++;
    }
    if (j > 0) {
      break;
    }
  }

  printf("Enter name of the person who is sick.\n");
  scanf(" %s", name_of_absent_employee);

  while (schedule[i - j].day == day && schedule[i - j].month == month) {
    if (strcmp(schedule[i - j].employee_name, name_of_absent_employee) == 0) {
      printf("Du valgte %s: %d: \n", schedule[i - j].employee_name, i);
      break;
    }
    i++;
  }
  if (j == 0) {
    printf("Invalid input\n");
  }

  schedule_fp = fopen("schedule.csv", "w");
  if (schedule_fp == NULL) {
    perror("Error: ");
    exit(EXIT_FAILURE);
  }

  printf("Enter name of employee covering shift\n");
  scanf(" %s", schedule[i - j].employee_name);
  fseek(schedule_fp, 0, SEEK_SET);
  for (i = 0; i < number_of_shifts; i++) {
    fprintf(schedule_fp, "%s,%d,%d,%.2f,%.2f,%s,%s,%d\n", schedule[i].weekday,
            schedule[i].day, schedule[i].month, schedule[i].shift_start,
            schedule[i].shift_end, schedule[i].employee_name,
            schedule[i].shift_position, schedule[i].youth_worker);
  }

  fclose(schedule_fp);
}

int check_if_employee_exists(employee_s employees[], int num_of_employees,
                             char scanned_employee_name[]) {

  int i, found_employee_bool = 0;
  char ch;
  while ((ch = getchar()) != '\n' && ch != EOF)
    continue;
  capitalize_string(scanned_employee_name);
  for (i = 0; i <= num_of_employees; i++) {
    if (!strcmp(scanned_employee_name,
                employees[i].name)) {
      found_employee_bool = 1;
      break;
    }
  }
  return found_employee_bool;
}

void change_schedule_file(schedule_s schedule[], FILE *schedule_fp,
                          int number_of_shifts, int i, int j) {
  printf("Enter name of employee covering shift\n ");
  scanf(" %s ", schedule[i - j].employee_name);
  fseek(schedule_fp, 0, SEEK_SET);
  for (i = 0; i < number_of_shifts; i++) {
    fprintf(schedule_fp, "%s,%d,%d,%.2f,%.2f,%s,%s\n", schedule[i].weekday,
            schedule[i].day, schedule[i].month, schedule[i].shift_start,
            schedule[i].shift_end, schedule[i].employee_name,
            schedule[i].shift_position);
  }
}

/*void print_schedule(schedule_s schedule[], int number_of_shifts) {
  int i, j, start_day, start_month, days_forward;

  printf("Enter the start date and end date you wish to see (dd/mm days):\n");
  scanf("%d/%d %d", &start_day, &start_month, &days_forward);

  for (i = 0; i < 100; i++) {
    if (start_day == schedule[i].day && start_month == schedule[i].month) {
      printf("%d %-9s %.2d/%.2d %.4d-%.4d %-10s %s \n", schedule[i].weekday,
             schedule[i].day, schedule[i].month, schedule[i].shift_start,
             schedule[i].shift_end, schedule[i].employee_name,
             schedule[i].shift_position);
    }
  }
} */
