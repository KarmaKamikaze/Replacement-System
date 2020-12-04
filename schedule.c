#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedule.h"
#include "replacer.h"

void schedule_function (void);
void fill_schedule_with_data (schedule_s schedule[], FILE *schedule_fp, int shifts);
void edit_schedule(schedule_s schedule[], FILE *schedule_fp, int number_of_shifts);
/* void change_schedule_file(schedule_s schedule[], FILE *schedule_fp, int number_of_shifts, int i, int j);
 */
void schedule_function(void) {
  schedule_s schedule[DAYS_IN_SCHEDULE];
  int number_of_shifts;
  FILE *schedule_fp = fopen("schedule.csv", "r");
  if (schedule_fp == NULL) {
    perror("Error: ");
    exit(EXIT_FAILURE);
  }
  number_of_shifts = count_elements(schedule_fp);
  fill_schedule_with_data(schedule, schedule_fp, number_of_shifts);
  /*   print_schedule (schedule, number_of_shifts); */
  fclose(schedule_fp);

  edit_schedule(schedule, schedule_fp, number_of_shifts);
}

void fill_schedule_with_data(schedule_s schedule[], FILE *schedule_fp, int number_of_shifts) {
  int i;
  char input_string[MAX_STRING_LENGTH];

  fseek(schedule_fp, 0, SEEK_SET);
  for (i = 0; i < number_of_shifts; i++) {
    fgets(input_string, MAX_LINE_LENGTH, schedule_fp);
    sscanf(input_string, "%[^,],%d,%d,%lf,%lf,%[^,],%[^\n]s,\n",
           schedule[i].weekday, &schedule[i].day,
           &schedule[i].month, &schedule[i].shift_start, &schedule[i].shift_end,
           schedule[i].employee_name, schedule[i].shift_position);
    schedule[i].weekday[MAX_WEEK_DAY_NAME - 1] = '\0';
    schedule[i].employee_name[MAX_STRING_LENGTH - 1] = '\0';
    schedule[i].shift_position[MAX_STRING_LENGTH - 1] = '\0';
  }

  /* for (i = 0; i < 25; i++){
    printf("\n\n%d %s %d %d %f %f %s %s\n\n", schedule[i].day_of_year, schedule[i].weekday, schedule[i].day,
           schedule[i].month, schedule[i].shift_start, schedule[i].shift_end,
           schedule[i].employee_name, schedule[i].shift_position);
  } */
}

void edit_schedule(schedule_s schedule[], FILE *schedule_fp, int number_of_shifts) {
  int i, j = 0;
  int day, month;
  char name[MAX_STRING_LENGTH];

  schedule_fp = fopen("schedule.csv", "w");
  if (schedule_fp == NULL) {
    perror("Error: ");
    exit(EXIT_FAILURE);
  }

  printf("Enter day and month for sick person.");
  scanf(" %d/%d", &day, &month);

  for (i = 0; i < MAX_STRING_LENGTH; i++) {
    while(schedule[i].day == day && schedule[i].month == month) {
      /*Don't think too much about it*/
      printf("vagt nr. %d Medarbejder: %s\n",j , schedule[i].employee_name);
      j++;
      i++;
    }
    if (j > 0) { break; }
    /* if(schedule[i].day == day && schedule[i].month == month)
    {
    } */
  }

  printf("Enter name of the person who is sick.\n");
  scanf(" %s", name);

  while(schedule[i-j].day == day && schedule[i-j].month == month) {
    if (strcmp(schedule[i-j].employee_name, name) == 0) {
      printf("Du valgte %s: %d: \n", schedule[i-j].employee_name, i);
      break;
    }
    i++;
  }
  if (j == 0)  { printf("Invalid input\n"); }
  
  printf("Enter name of employee covering shift\n");
  scanf(" %s",schedule[i-j].employee_name);
  fseek(schedule_fp, 0, SEEK_SET);
  for (i = 0; i < number_of_shifts; i++) {
    fprintf(schedule_fp, "%s,%d,%d,%.2f,%.2f,%s,%s\n",
          schedule[i].weekday, schedule[i].day,
          schedule[i].month, schedule[i].shift_start, schedule[i].shift_end,
          schedule[i].employee_name, schedule[i].shift_position);
  }

/*   change_schedule_file(schedule, schedule_fp, number_of_shifts, i, j);
 */
  fclose(schedule_fp);
}

/* void change_schedule_file(schedule_s schedule[], FILE *schedule_fp, int number_of_shifts, int i, int j) {
  printf("Enter name of employee covering shift\n");
  scanf(" %s",schedule[i-j].employee_name);
  fseek(schedule_fp, 0, SEEK_SET);
  for (i = 0; i < number_of_shifts; i++) {
    fprintf(schedule_fp, "%s,%d,%d,%.2f,%.2f,%s,%s\n",
          schedule[i].weekday, schedule[i].day,
          schedule[i].month, schedule[i].shift_start, schedule[i].shift_end,
          schedule[i].employee_name, schedule[i].shift_position);
  }
} */

/* void print_schedule (schedule_s schedule[], int number_of_shifts) {
  int i, j, start_day, start_month, days_forward;

  printf("Enter the start date and end date you wish to see (dd/mm days):\n");
  scanf("%d/%d %d", &start_day, &start_month, &days_forward);

  for (i = 0; i < 100; i++){
    if(start_day == schedule[i].day && start_month == schedule[i].month){
      printf("%d %-9s %.2d/%.2d %.4d-%.4d %-10s %s \n", schedule[i].day_of_year, schedule[i].weekday, schedule[i].day, schedule[i].month,
          schedule[i].shift_start, schedule[i].shift_end,
          schedule[i].employee_name, schedule[i].shift_position);
    }
  }
} */
