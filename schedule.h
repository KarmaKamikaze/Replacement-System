#include "employee.h"
#include "utility.h"
#include <stdio.h>

#ifndef SCHEDULE_H
#define SCHEDULE_H

#define MAX_WEEK_DAY_NAME 10
#define SHIFTS_IN_SCHEDULE 1500
#define DAYS_A_YEAR 365
#define MAX_NUMBER_OF_SHIFTS 5000

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

void find_replacement(employee_s employees[], int num_of_employees,
                      int num_of_total_positions);

#endif
