#include "rules.h"
#include "employee.h"
#include "schedule.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_for_rules(employee_s *employee, schedule_s schedule[], int shift,
                    int number_of_shifts, int day, int month);
int check_what_shift_employee_has_this_day(employee_s *employee,
                                           schedule_s schedule[], int shift,
                                           int day, int month,
                                           int days_from_shift);
int check_for_11_hour_rule(employee_s *employee, schedule_s schedule[],
                           int shift, int day, int month);
int check_for_48_hour_rule(employee_s *employee, schedule_s schedule[],
                           int number_of_shifts, int current_shift, int month);
double total_hours_worked(employee_s *employee, schedule_s schedule[],
                          int number_of_shifts);
double convert_minutes_to_fractions(double minutes);
bool check_for_weekly_day_off(employee_s *employee, schedule_s schedule[], int shift, int day, int month);


void check_for_weekday_availability(employee_s *possible_replacement,
                                    schedule_s absentee_shift_in_schedule);
bool shift_is_weekday(schedule_s absentee_shift_in_schedule);
void check_for_youth_worker(employee_s *possible_replacement,
                            schedule_s absentee_shift_in_schedule);
void check_for_qualifications(employee_s *possible_replacement, schedule_s schedule,
                              int num_of_total_positions);


int days_in_month(int month);
date_s tomorrow(date_s date);
date_s yesterday(date_s date);

void sort_replacements(employee_s possible_replacements[],
                       int remaining_employees);
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

int check_for_rules(employee_s *employee, schedule_s schedule[], int shift,
                    int number_of_shifts, int day, int month) {
  /*Checks whether employee does not breach 11-hour rule, 48-hour rule and
   * weekly day off. If not breaching, prints employee and phone number*/
  /*   */
  if (check_for_11_hour_rule(employee, schedule, shift, day, month) &&
      check_for_48_hour_rule(employee, schedule, number_of_shifts, shift,
                             month) &&
      check_for_weekly_day_off(employee, schedule, shift, day, month)) {
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
    while (schedule[shift].day != date.day &&
           schedule[shift].month == date.month) {
      shift--;
    }
    shift++;
  } else if (days_from_shift > 0) {
    for (i = 0; i < days_from_shift; i++) {
      date = tomorrow(date);
    }
    while (schedule[shift].day != date.day &&
           schedule[shift].month == date.month) {
      shift++;
    }
  }

  /*In case of 1st of January, set shift to 0 if looking for shift yesterday and
   * today*/
  /*Runs through all shift on the specified day and checks if employee has a
   * shift, then returns the shift number*/
  while (schedule[shift].day == day + days_from_shift &&
         schedule[shift].month == month) {
    if (!strcmp(schedule[shift].employee_name, employee->name)) {
      return shift;
    }
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
   * checks whether employee has at least 11 hours of rest if above 18 and 12
   * hours if youth worker, between that ending, and shift on specified day
   * starting*/
  if ((found_shift = check_what_shift_employee_has_this_day(
           employee, schedule, shift, day, month, -1)) != SHIFT_NOT_FOUND) {
    /*Check for if previous_shift_end ends after midnight*/
    if ((24 - schedule[found_shift].shift_end + absentee_shift_start) <
        (employee->youth_worker == 1 ? 12 : 11)) {
      return false;
    }
  }
  /*If shift was found day after,*/
  if ((found_shift = check_what_shift_employee_has_this_day(
           employee, schedule, shift, day, month, 1)) != SHIFT_NOT_FOUND) {
    if ((24 - absentee_shift_end + schedule[found_shift].shift_start) <
        (employee->youth_worker == 1 ? 12 : 11)) {
      return false;
    }
  }

  /*If shift was not found on the specified day of shift - returns true.
   * If shift was found; checks whether employee has at least 11 hour rest
   * between that starting and shift on specified day ending */
  if ((found_shift = check_what_shift_employee_has_this_day(
           employee, schedule, shift, day, month, 0)) == SHIFT_NOT_FOUND) {
    return true;
  } else {
    if ((schedule[found_shift].shift_start - absentee_shift_end) <
            (employee->youth_worker == 1 ? 12 : 11) &&
        (absentee_shift_start - schedule[found_shift].shift_end) <
            (employee->youth_worker == 1 ? 12 : 11)) {
      return false;
    }
  }
  return true;
}

/** @brief Function returns true if employee has less than 48 hours on a weekly
 * average in 4 months.
 *
 * @param employee Parse in the employee choosen.
 * @param schedule Parse in the schedule array containing all shifts.
 * @param number_of_shifts Parse in the number of shifts.
 * @return true or false.
 */
int check_for_48_hour_rule(employee_s *employee, schedule_s schedule[],
                           int number_of_shifts, int current_shift, int month) {
  int i, days_in_4_months = 0;
  double hours_worked = 0, average_hours_worked = 0;

  /* Counts days in a 4 month period. */
  for (i = month; i <= month + 3; i++) {
    days_in_4_months += days_in_month(i);
  }
  /* loops through all shifts and counts the total hours worked in 4 months for
   * employee. */
  hours_worked = total_hours_worked(employee, schedule, number_of_shifts);

  /* calculates averages weekly hours worked for employee. */
  average_hours_worked = (hours_worked / days_in_4_months) * WEEK_LENGTH;
  if (average_hours_worked > (employee->youth_worker ? 40 : 48)) {
    return false;
  }
  return true;
}

/**
 * @brief Funtion returns a total hours worked for an employee as a double.
 *
 * @param employee Parse in the employee choosen.
 * @param schedule Parse in the schedule array containing all shifts.
 * @param number_of_shifts Number of shifts in schedule.
 * @return double
 */
double total_hours_worked(employee_s *employee, schedule_s schedule[],
                          int number_of_shifts) {
  int i;
  double hours_worked = 0;

  for (i = 0; i < number_of_shifts; i++) {
    if (strcmp(schedule[i].employee_name, employee->name) == 0) {
      hours_worked += (int)schedule[i].shift_end - (int)schedule[i].shift_start;
      hours_worked += convert_minutes_to_fractions(schedule[i].shift_end) -
                      convert_minutes_to_fractions(schedule[i].shift_start);
    }
  }
  return hours_worked;
}

double convert_minutes_to_fractions(double minutes) {
  double fraction = minutes - (int)minutes;
  return (fraction == 0
              ? 0
              : ((fraction > 0.14 && fraction < 0.16)
                     ? 0.25
                     : ((fraction > 0.29 && fraction < 0.31) ? 0.5 : 0.75)));
}

/**
 * @brief Checks if employee can take a shift and still get a weekly day off. returns true if they can
 * @param employee the specific employee checked
 * @param schedule array of structs of shifts in schedule.
 * @param shift shift number.
 * @param day day of date
 * @param month month of date
 * @return bool
 * */
bool check_for_weekly_day_off(employee_s *employee, schedule_s schedule[], int shift, int day, int month){
  int i = -1,j = 1,count = 0;
  bool shift_not_found_days_before = false , shift_not_found_days_after = false;
  /*While loop that runs as long as it hasn't found 5/6 consecutive days (5 for youthworkers, 6 for non-youthworkers) where the employee has a shift from the sick leave day - 
   *or if it it hasn't finished looking through the days*/
  while (count < (employee->youth_worker ? 5 : 6) && (shift_not_found_days_before == false || shift_not_found_days_after == false)){
    /*looking through the days before the shift*/
    if (!shift_not_found_days_before && check_what_shift_employee_has_this_day(employee, schedule, shift, day, month, i) != SHIFT_NOT_FOUND)
      count++;
    else
      shift_not_found_days_before = true;
    /*looking through the days after the shift*/
    if (!shift_not_found_days_after && check_what_shift_employee_has_this_day(employee, schedule, shift, day, month, j) != SHIFT_NOT_FOUND)
      count++;
    else
      shift_not_found_days_after = true;
    i--;
    j++;
  }
  if (count >= (employee->youth_worker ? 5 : 6))
    {
      return false;
    }
  return true;
}


/**
 * @brief gives points based on an employee's and the absentee's weekday availability
 * @param possible_replacement the employee that is a possible replacement
 * @param absentee_shift_in_schedule Shift that needs a replacement.
 * @return void
 * */
void check_for_weekday_availability(employee_s *possible_replacement,
                                    schedule_s absentee_shift_in_schedule) {
  if (!possible_replacement->weekday_availability &&
      shift_is_weekday(absentee_shift_in_schedule))
    possible_replacement->points += 2; /*non-weekday available worker for sick weekday available worker*/
  else if (possible_replacement->weekday_availability &&
            !shift_is_weekday(absentee_shift_in_schedule))
    possible_replacement->points += 3; /*weekday available worker for sick non-weekday available worker*/
  else
    possible_replacement->points += 7; /*weekday available worker for sick weekday available worker
                                        *non-weekday available worker for sick non-weekday available worker*/
}
/**
 * @brief a function that checks if a shift is a weekday shift or not
 * @param absentee_shift_in_schedule the absentee's shift
 * @return bool
 * */
bool shift_is_weekday(schedule_s absentee_shift_in_schedule) {
  return (strcmp(absentee_shift_in_schedule.weekday, "SATURDAY") &&
          strcmp(absentee_shift_in_schedule.weekday, "SUNDAY") &&
          absentee_shift_in_schedule.shift_start < 16.00);
}

/**
 * @brief gives points based on if the employee and the absentee is a youth worker or not
 * @param possible_replacement the employee that is a possible replacement
 * @param absentee_shift_in_schedule Shift that needs a replacement.
 * @return void
 * */
void check_for_youth_worker(employee_s *possible_replacement,
                            schedule_s absentee_shift_in_schedule) {
  if (!possible_replacement->youth_worker &&
      absentee_shift_in_schedule.youth_worker)
    possible_replacement->points += 2; /*non-youthworker for sick youthworker*/
  else if (possible_replacement->youth_worker &&
            !absentee_shift_in_schedule.youth_worker)
    possible_replacement->points += 15; /*youthworker for sick non-youthworker*/
  else
    possible_replacement->points += 8; /*both youthworker for sick youthworker and non-youthworker for
              sick non-youthworker*/
}

/**
 * @brief Compares an employees qualifications to the qualifications needed to
 * cover a shift. Assigns points to employees with qualifications.
 * @param possible_replacement the employee that is a possible replacement
 * @param absentee_shift_in_schedule Shift that needs a replacement.
 * @param num_of_total_positions Total number of qualifications existing.
 */
void check_for_qualifications(employee_s *possible_replacement,
                              schedule_s absentee_shift_in_schedule,
                              int num_of_total_positions) {
  int i;
  for (i = 0; i < possible_replacement->number_of_positions; i++) {
    if (!strcmp(possible_replacement->positions[i],
                absentee_shift_in_schedule.shift_position)) {
      possible_replacement->points += 2 * num_of_total_positions;
      possible_replacement->points -= (possible_replacement->number_of_positions - 1);
      possible_replacement->is_qualified = true;
    }
  }
}

/**
 * @brief Function returns the day after the day parsed in.
 *
 * @param date Date parsed in
 * @return date, returns the day after the day parsed in.
 */
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

/**
 * @brief Function returns the day before the day parsed in.
 *
 * @param date Date parsed in
 * @return date, returns the day before the day parsed in.
 */
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

/**
 * @brief Funtion returns the days in a specific month.
 *
 * @param month input the month you want to find the days in.
 * @return int, return days in a specific month.
 */
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

void sort_replacements(employee_s possible_replacements[],
                       int remaining_employees) {
  qsort(possible_replacements, remaining_employees, sizeof(employee_s),
        compare_replacements);
}

int compare_replacements(const void *a, const void *b) {
  employee_s *p1 = (employee_s *)a;
  employee_s *p2 = (employee_s *)b;

  return -(p1->points - p2->points);
}
