#include <stdio.h>
#include <stdlib.h>
#include "schedule.h"

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
    sscanf(input_string, "%[^,],%d,%d,%d,%d,%[^,],%s,%*[^\n]\n", schedule[i].weekday,
           &schedule[i].day, &schedule[i].month,
           &schedule[i].shift_start, &schedule[i].shift_end,
           schedule[i].employee_name, schedule[i].shift_position);
    schedule[i].weekday[MAX_WEEK_DAY_NAME - 1] = '\0';
  }
  fclose(schedule_fp);

}


void print_schedule (schedule_s schedule[], int shifts) {
  int i;

  for (i = 0; i < shifts; i++) {
    printf("%s %d %d %.4d %.4d %s %s \n", schedule[i].weekday,
           schedule[i].day, schedule[i].month,
           schedule[i].shift_start, schedule[i].shift_end,
           schedule[i].employee_name, schedule[i].shift_position);
  }

}

