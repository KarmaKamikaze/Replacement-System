#include "employee.h"
#include "schedule.h"
#include "utility.h"

#ifndef RULES_H
#define RULES_H

#define SHIFT_NOT_FOUND -1 /*Sentinel value if no shift is found*/
#define WEEK_LENGTH 7

typedef struct date_s {
  int day;
  int month;
} date_s;

bool check_for_rules(employee_s *employee, schedule_s schedule[], int shift,
                     int number_of_shifts, int day, int month);

void check_for_weekday_availability(employee_s *possible_replacement,
                                    schedule_s absentee_shift_in_schedule);
void check_for_youth_worker(employee_s *possible_replacement,
                            schedule_s absentee_shift_in_schedule);
void check_for_qualifications(employee_s *possible_replacement,
                              schedule_s schedule, int num_of_total_positions);
void sort_replacements(employee_s possible_replacements[],
                       int remaining_employees);
#endif
