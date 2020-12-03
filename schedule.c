#include "replacer.h"
#include <stdio.h>
#include <stdlib.h>
#include "schedule.h"

/* void schedule_function (void);
void fill_schedule_with_data (schedule_s schedule[], FILE *schedule_fp, int shifts);
int count_shifts (FILE *schedule_fp);
void print_schedule (schedule_s schedule[], int shifts); */

void schedule_function(void) {
  schedule_s schedule[DAYS_IN_SCHEDULE];
  FILE *schedule_fp = NULL;
  int number_of_shifts;
  schedule_fp = fopen("schedule.csv", "r");
  if (schedule_fp == NULL) {
    perror("Error (41): ");
    exit(EXIT_FAILURE);
  }
  number_of_shifts = count_elements(schedule_fp);
  fill_schedule_with_data(schedule, schedule_fp, number_of_shifts);
  /*   print_schedule (schedule, number_of_shifts
  ); */
  fclose(schedule_fp);
}

void fill_schedule_with_data(schedule_s schedule[], FILE *schedule_fp,
                             int number_of_shifts) {
  int i;
  char input_string[MAX_STRING_LENGTH];

  fseek(schedule_fp, 0, SEEK_SET);
  for (i = 0; i < number_of_shifts; i++) {
    fgets(input_string, MAX_LINE_LENGTH, schedule_fp);
    sscanf(input_string, "%d,%[^,],%d,%d,%lf,%lf,%[^,],%[^\n]s,\n",
           &schedule[i].day_of_year, schedule[i].weekday, &schedule[i].day,
           &schedule[i].month, &schedule[i].shift_start, &schedule[i].shift_end,
           schedule[i].employee_name, schedule[i].shift_position);
    schedule[i].weekday[MAX_WEEK_DAY_NAME - 1] = '\0';
  }
  fclose(schedule_fp);
}

/* Denne del ikke ikke færdig endnu! */
/* void print_schedule (schedule_s schedule[], int number_of_shifts
) {
  int i, j, start_day, start_month, days_forward;

  printf("Enter the start date and end date you wish to see (dd/mm days):\n");
  scanf("%d/%d %d", &start_day, &start_month, &days_forward);

  for (i = 0; i < 100; i++){ /* 100 is number of number_of_shifts
  . temporary and should be changed!! */
/* if(start_day == schedule[i].day && start_month == schedule[i].month){
  printf("%d %-9s %.2d/%.2d %.4d-%.4d %-10s %s \n", schedule[i].day_of_year,
schedule[i].weekday, schedule[i].day, schedule[i].month,
        schedule[i].shift_start, schedule[i].shift_end,
        schedule[i].employee_name, schedule[i].shift_position);
}
}
}  */

/*  int separator[] = {"+-------------------------\0"};
  printf("\n\n\n\n%s%s%s%s+\n", separator, separator, separator, separator);
  printf("|%-25s|%-25s|%-25s|%-25s|\n", "TEAM NAME", "POINTS", "GOALS SCORED BY
  TEAM", "GOALS SCORED AGAINST TEAM"); for (i = 0; i < number_teams; i++) {
  printf("%s%s%s%s+\n", separator, separator, separator, separator);
  printf("|%-25s|%-25d|%-25d|%-25d|\n",
  teams[i].name, teams[i].points, teams[i].goals_by_team,
  teams[i].goals_against_team);
  }
  printf("%s%s%s%s+\n\n\n\n", separator, separator, separator, separator); */
