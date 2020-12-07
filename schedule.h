#include "employee.h"
#include <stdio.h>

#ifndef SCHEDULE_H
#define SCHEDULE_H

#define MAX_WEEK_DAY_NAME 10
#define DAYS_IN_SCHEDULE 120
#define MAX_LINE_LENGTH 500
#define MAX_STRING_LENGTH 100
#define DAYS_A_YEAR 365

typedef struct schedule_s {
  char weekday[MAX_WEEK_DAY_NAME];
  int day;
  int month;
  double shift_start;
  double shift_end;
  char employee_name[MAX_STRING_LENGTH];
  char shift_position[MAX_STRING_LENGTH];
  int youth_worker;

} schedule_s;

void find_replacement(employee_s employees[], int num_of_employees);

#endif
