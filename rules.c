#include "rules.h"
#include "employee.h"
#include "schedule.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_for_rules(employee_s *employee, schedule_s schedule[], int shift,
                    int day, int month);
int check_what_shift_employee_has_this_day(employee_s *employee,
                                           schedule_s schedule[], int shift,
                                           int day, int month,
                                           int days_from_shift);
int check_for_11_hour_rule(employee_s *employee, schedule_s schedule[],
                           int shift, int day, int month);
int check_for_48_hour_rule(employee_s *employee);
int check_for_weekly_day_off(employee_s *employee);

void check_for_qualifications(employee_s possible_replacements[], int remaining_employees, schedule_s schedule, int num_of_total_positions);

int days_in_month(int month);
date_s tomorrow(date_s date);
date_s yesterday(date_s date);

void sort_replacements(employee_s possible_replacements[], int remaining_employees);
int compare_replacements(const void *a, const void *b);

/**
 * @brief Checks if employee does not breach any rules or legislature if they
 * were to cover the shift.
 *
 * @param employee specific element (meaning specific employee) of array of
 * structs of employees.
 * @param schedule array of structs of shifts.
 * @param shift number shift.
 * @param day day of date.
 * @param month month of the date.
 * @return int
 */
int check_for_rules(employee_s *employee, schedule_s schedule[], int shift, int day, int month) {
  /*Checks whether employee does not breach 11-hour rule, 48-hour rule and weekly day off.
  * If not breaching, prints employee and phone number*/
/*   */
 if (check_for_11_hour_rule(employee, schedule, shift, day, month) && check_for_48_hour_rule(employee) && check_for_weekly_day_off(employee)) {
    return true;
  }

  return false;
}

/**
 * @brief Checks if employee has shift on the specified days since the
 * absentee's shift.
 *
 * @param employee the specific employee checked
 * @param schedule array of structs of shifts.
 * @param shift number shift.
 * @param day day of date.
 * @param month month of date.
 * @param days_from_shift number of days from the absentee's shift to look if
 * employee has a shift.
 * @return int. number of shifts if employee has a shift on specified day.
 * Otherwise returns false.
 */
int check_what_shift_employee_has_this_day(employee_s *employee,
                                           schedule_s schedule[], int shift,
                                           int day, int month,
                                           int days_from_shift) {
  date_s date;
  int i;
  date.day = day;
  date.month = month;
  if (days_from_shift <= 0) {
    for (i = 0; i >= days_from_shift; i--) {
      date = yesterday(date);
    }
    while (schedule[shift].day != date.day && schedule[shift].month == date.month) {shift--;}
    shift++;
  }
  else if (days_from_shift > 0) {
    for (i = 0; i < days_from_shift; i++){
      date = tomorrow(date);
    }
    while (schedule[shift].day != date.day  && schedule[shift].month == date.month) {shift++;}
  }

  /*In case of 1st of January, set shift to 0 if looking for shift yesterday and today*/
  /*Runs through all shift on the specified day and checks if employee has a shift, then returns the shift number*/
  while (schedule[shift].day == day + days_from_shift && schedule[shift].month == month) {
    if (!strcmp(schedule[shift].employee_name,employee->name)) {
      return shift;}
    shift++;
  }
  return SHIFT_NOT_FOUND;
}

/**
 * @brief Checks if employee has at least 11 hour rest in-between shifts.
 *
 * @param employee the specific employee checked
 * @param schedule array of structs of shifts.
 * @param shift number shift.
 * @param day day of date
 * @param month month of date
 * @return int
 */
int check_for_11_hour_rule(employee_s *employee, schedule_s schedule[],
                           int shift, int day, int month) {
  int found_shift;
  double absentee_shift_start = schedule[shift].shift_start,
         absentee_shift_end = schedule[shift].shift_end;
  /*If shift was found day before shift,
   * checks whether employee has at least 11 hour rest between that ending,
   * and shift on specified day starting*/
  if ((found_shift = check_what_shift_employee_has_this_day(
           employee, schedule, shift, day, month, -1)) != SHIFT_NOT_FOUND) {
    /*Check for if previous_shift_end ends after midnight*/
    /*Does not check if it is comparing itself with the employees first shift -
     * this might not be implemented - because prototype.*/
    if ((24 - schedule[found_shift].shift_end + absentee_shift_start) < 11) {
      return false;
    }
  }

  /*If shift was found day after,
   * checks whether employee has at least 11 hour rest between that ending,
   * and shift on specified day starting*/
  if ((found_shift = check_what_shift_employee_has_this_day(
           employee, schedule, shift, day, month, 1)) != SHIFT_NOT_FOUND) {
    if ((24 - absentee_shift_end + schedule[found_shift].shift_start) < 11) {
      return false;
    }
  }

  /*If shift was not found on the specified day of shift - returns true.
  * If shift was found; checks whether employee has at least 11 hour rest
  * between that starting and shift on specified day ending */
  if ((found_shift = check_what_shift_employee_has_this_day(employee, schedule, shift, day, month, 0)) == SHIFT_NOT_FOUND) {return true;}
  else {
    if ((schedule[found_shift].shift_start - absentee_shift_end) < 11 && (absentee_shift_start - schedule[found_shift].shift_end) < 11) {
      return false;
    }
  }
  return true;
}

int check_for_48_hour_rule(employee_s *employee) { return true; }

int check_for_weekly_day_off(employee_s *employee) { return true; }

void check_for_qualifications(employee_s possible_replacements[], int remaining_employees, schedule_s schedule, int num_of_total_positions){
  int i, j;

  for (i = 0; i < remaining_employees; i++){
    for (j = 0; j < possible_replacements[i].number_of_positions; j++) {
      if (!strcmp(possible_replacements[i].positions[j], schedule.shift_position)) {
        possible_replacements[i].points += num_of_total_positions;
        possible_replacements[i].points -= (possible_replacements[i].number_of_positions - 1);
      }
    }
  }
}

date_s tomorrow(date_s date) {
  if (date.month == 12 && date.day == days_in_month(date.month)) {
    date.month = 1;
    date.day = 1;
  } else if (date.day == days_in_month(date.month)) {
    date.month++;
    date.day = 1;
  } else {
    date.day++;
  }

  return date;
}

date_s yesterday(date_s date) {
  if (date.month == 1 && date.day == 1) {
    date.month = 12;
    date.day = 31;
  } else if (date.day == 1) {
    date.month--;
    date.day = days_in_month(date.month);
  } else {
    date.day--;
  }

  return date;
}

int days_in_month(int month) {
  switch (month) {
  case 1:
  case 3:
  case 5:
  case 7:
  case 8:
  case 10:
  case 12:
    return 31;
  case 2:
      return 28;
  case 4:
  case 6:
  case 9:
  case 11:
    return 30;

  default:
    return 0;
  }
}

void sort_replacements(employee_s possible_replacements[], int remaining_employees){
  qsort(possible_replacements, remaining_employees, sizeof(employee_s), compare_replacements);
}


int compare_replacements(const void *a, const void *b){
  employee_s *p1 = (employee_s*) a;
  employee_s *p2 = (employee_s*) b; 

  return -( p1->points- p2->points);
}
