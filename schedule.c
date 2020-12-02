#include <stdio.h>
#include <stdlib.h>
/* #include "schedule.h" */

/* ~~ SYMBOLIC CONSTANTS ~~ */
#define MAX_WEEK_DAY_NAME 10
#define DAYS_IN_SCHEDULE  120
#define MAX_LINE_LENGTH 500
#define MAX_STRING_LENGTH 100

/* ~~ STRUCTS ~~ */
typedef struct schedule_s {
  char weekday[MAX_WEEK_DAY_NAME];
  int day;
  int month;
  int shift_start; /* 0100, 1630, etc. */
  int shift_end;
  char employee_name[MAX_STRING_LENGTH];
  char shift_position[MAX_STRING_LENGTH];

} schedule_s;

/* ~~ PROTOTYPES ~~ */
void schedule_function (void);
FILE *open_schedule_file (FILE *schedule_fp);
void fill_schedule_with_data (schedule_s schedule[], FILE *schedule_fp, int shifts);
int count_shifts (FILE *schedule_fp);
void print_schedule (schedule_s schedule[], int shifts);

void schedule_function (void) {
  schedule_s schedule[DAYS_IN_SCHEDULE];
  FILE *schedule_fp = NULL;
  int shifts;
  schedule_fp = open_schedule_file (schedule_fp);
  shifts = count_shifts(schedule_fp);
  fill_schedule_with_data (schedule, schedule_fp, shifts);
  print_schedule (schedule, shifts);
  
}

FILE *open_schedule_file (FILE *schedule_fp) {
  schedule_fp = fopen("schedule.csv", "r");
  if (schedule_fp == NULL) {
    perror("Error (41): ");
    exit(EXIT_FAILURE);
  }
  return schedule_fp;
}

int count_shifts (FILE *schedule_fp) {
  int shifts = 0, ch = 0;
  /* Makes sure we ALWAYS read from the start of our file. */
  fseek(schedule_fp, 0, SEEK_SET);
  while (ch != EOF) {
    ch = fgetc(schedule_fp);
    if (ch == '\n') {
      shifts++;
    }
  }
  return shifts;
}

void fill_schedule_with_data (schedule_s schedule[], FILE *schedule_fp, int shifts) {
  int i;
  char input_string[MAX_STRING_LENGTH];

  fseek(schedule_fp, 0, SEEK_SET);
  for (i = 0; i < shifts; i++) {
    fgets(input_string, MAX_LINE_LENGTH, schedule_fp);
    sscanf(input_string, "%[^,],%d,%d,%d,%d,%[^,],%[^\n]s,\n", schedule[i].weekday,
           &schedule[i].day, &schedule[i].month,
           &schedule[i].shift_start, &schedule[i].shift_end,
           schedule[i].employee_name, schedule[i].shift_position);
    schedule[i].weekday[MAX_WEEK_DAY_NAME - 1] = '\0';
  }
  fclose(schedule_fp);

}


void print_schedule (schedule_s schedule[], int shifts) {
  int i;

/*   for (i = 0; i < shifts; i++) {
    printf("%-9s %.2d/%.2d %.4d-%.4d %-10s %s \n", schedule[i].weekday,
           schedule[i].day, schedule[i].month,
           schedule[i].shift_start, schedule[i].shift_end,
           schedule[i].employee_name, schedule[i].shift_position);
  } */

}

int seperator[] = {"+-------------------------\0"};
int start_day, start_month, end_day, end_month;

  printf("Enter the start date and end date you wish to see (dd/mm-dd/mm):\n");
  scanf("%d/%d-%d/%d", start_day, start_month, end_day, end_month);




int separator[] = {"+-------------------------\0"};
  printf("\n\n\n\n%s%s%s%s+\n", separator, separator, separator, separator);
  printf("|%-25s|%-25s|%-25s|%-25s|\n", "TEAM NAME", "POINTS", "GOALS SCORED BY TEAM", "GOALS SCORED AGAINST TEAM");
  for (i = 0; i < number_teams; i++) {
  printf("%s%s%s%s+\n", separator, separator, separator, separator);
  printf("|%-25s|%-25d|%-25d|%-25d|\n",
  teams[i].name, teams[i].points, teams[i].goals_by_team, teams[i].goals_against_team);
  }
  printf("%s%s%s%s+\n\n\n\n", separator, separator, separator, separator);