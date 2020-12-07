#include "employee.h"
#include "schedule.h"
#include "rules.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


int check_for_rules(employee_s employee, schedule_s schedule[], int shift, int day, int month);
int check_if_employee_has_shift_this_day (employee_s employee, schedule_s schedule[], int shift, int *off_set, int day, int month, char checked_day[MAX_DETERMINANT_LENGTH]);
int check_for_11_hour_rule(employee_s employee, schedule_s schedule[], int shift, int day, int month);
int check_for_48_hour_rule(employee_s employee);
int check_for_weekly_day_off(employee_s employee);


/**
 * @brief Checks if employee does not breach any rules or legislature if they were to cover the shift.
 * 
 * @param employee specific element (meaning specific employee) of array of structs of employees.
 * @param schedule array of structs of shifts.
 * @param shift number shift.
 * @param day day of date.
 * @param month month of the date.
 * @return int 
 */
int check_for_rules(employee_s employee, schedule_s schedule[], int shift, int day, int month) {
/*if (!check_if_employee_has_shift_this_day(employee, schedule, shift, 0, day, month, "today")){
    return false;
  } */

  /*Checks whether employee does not breach 11-hour rule, 48-hour rule and weekly day off.
  * If not breaching, prints employee and phone number*/
  if (check_for_11_hour_rule(employee, schedule, shift, day, month) && check_for_48_hour_rule(employee) && check_for_weekly_day_off(employee)) {
/*     printf("passed truth check\n");
 */    printf("%s     %s\n", employee.name, employee.phone_number);
    return true;
  }
/*   printf("failed truth check\n");
 */  return false;
}

/**
 * @brief Checks if employee has shift yesterday, today or tomorrow.
 * 
 * @param employee the specific employee checked
 * @param schedule array of structs of shifts.
 * @param shift number shift.
 * @param off_set how much shift needs to be off set in functions which require shift to go back to the inputted value.
 * @param day day of date
 * @param month month of date
 * @param checked_day Whether function checks if employee has shift yesterday, today or tomorrow.
 * @return int. number of shifts if employee has a shift on specified day. Otherwise returns false.
 */
int check_if_employee_has_shift_this_day (employee_s employee, schedule_s schedule[], int shift, int *off_set, int day, int month, char checked_day[MAX_DETERMINANT_LENGTH]) {
  int switch_determinant;

  /*Determines whether to run switch case for yesterday, today or tomorrow*/
  if (strcmp(checked_day, "yesterday") == 0){
/*     printf("yesterday\n");
 */    switch_determinant = yesterday;
  }

  else if (!strcmp(checked_day, "today")){
/*     printf("today\n");
 */    switch_determinant = today;
  }
  else if (!strcmp(checked_day, "tomorrow")){
    switch_determinant = tomorrow;
/*     printf("tomorrow\n");
 */  }
  else {
    printf("Error in checking day. Terminating progam...\n");
    exit(EXIT_FAILURE);
  }

  switch (switch_determinant) {
  
  case yesterday:
    /*Checks if employee has shift yesterday*/
/*     printf("shift: %d\n", shift);
 */   
      /*Finds last shift two day ago and goes 1 shift forward to find first shift from day before*/
      while (schedule[shift].day != day - 2 && schedule[shift].month == month) {shift--;    /* printf("shift: %d\n", shift); */
}
      shift++;
/*    printf("shift: %d\n", shift); */
      /*Runs through all shifts yesterday and figures out if checked employee had a shift*/
      while (schedule[shift].day == day - 1 && schedule[shift].month == month) {
        if (!strcmp(schedule[shift].employee_name,employee.name)) {
          return shift;
        }
        shift++;
        off_set++;
      }
      return SHIFT_NOT_FOUND;

  case today:
    /*Checks if employee has shift today*/
/*    printf("shift: %d\n", shift);
      /*Finds last shift yesterday and goes 1 shift forward to find first shift today*/
      while (schedule[shift].day != day - 1 && schedule[shift].month == month) {shift--;    /* printf("shift: %d\n", shift) */;
}     shift++;
/*    printf("shift: %d\n", shift);*/

      /*Runs through all shifts today and figures out if checked employee has a shift*/
      while (schedule[shift].day == day && schedule[shift].month == month) {
        if (!strcmp(schedule[shift].employee_name,employee.name)) {
          return shift;
        }
        shift++;
        off_set++;
      }
      return SHIFT_NOT_FOUND;

  case tomorrow:
      /*Checks if employee has shift tomorrow*/
/*    printf("shift: %d\n", shift);
 */   
      /*Finds first shift tomorrow*/
      while (schedule[shift].day != day + 1 && schedule[shift].month == month) {shift++;    /* printf("shift: %d\n", shift); */
}
      /*Runs through all shifts tomorrow and figures out if checked employee has a shift*/
      while (schedule[shift].day == day + 1 && schedule[shift].month == month) {
        if (!strcmp(schedule[shift].employee_name,employee.name)) {
          return shift;
        }
        shift++;
        off_set++;
      }
      return SHIFT_NOT_FOUND;

  default:
          printf("ERROR IN DAY SELECTION\n");
          return false;
  }

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
int check_for_11_hour_rule(employee_s employee, schedule_s schedule[], int shift, int day, int month){
  int temp_shift_holder, off_set = 0;
  double previous_shift_end = 0, next_shift_start = 24,
         absentee_shift_start = schedule[shift].shift_start,
         absentee_shift_end = schedule[shift].shift_end;
/*   printf("shift day/month: %d/%d = actual day & month: %d/%d\n", schedule[shift].day, schedule[shift].month, day, month);
  printf("checked shift start: %f\n checked shift end: %f\n", absentee_shift_start, absentee_shift_end);
  printf("shift: %d\n", shift); */
  
  temp_shift_holder = check_if_employee_has_shift_this_day(employee, schedule, shift, &off_set, day, month, "yesterday");

  /*If shift was found yesterday,
  * checks whether employee has at least 11 hour rest between that ending,
  * and shift on specified day starting*/
  if (temp_shift_holder != SHIFT_NOT_FOUND){
    shift = temp_shift_holder;
/*     printf("inside if: 1\n"); */
    previous_shift_end = schedule[shift].shift_end;
/*     printf("prev shift end:%f\n", schedule[shift].shift_end);
    printf("temp_shift_holder: %d\n", shift); */

      /*Check for if previous_shift_end ends after midnight*/
      /*Does not check if it is comparing itself with the employees first shift - this might not be implemented - because prototype.*/
/*     printf("%s: 24 - %f + %f\n", employee.name, previous_shift_end, absentee_shift_start);
 */    if ((24 - previous_shift_end + absentee_shift_start) < 11) {
      return false;
    }
  }

  temp_shift_holder = check_if_employee_has_shift_this_day(employee, schedule, shift, &off_set, day, month, "tomorrow") - off_set;
/*   printf("shift: %d\n", shift);
 */
  /*If shift was found today,
  * checks whether employee has at least 11 hour rest between that ending,
  * and shift on specified day starting*/
  if (temp_shift_holder != SHIFT_NOT_FOUND){
    shift = temp_shift_holder;
/*     printf("inside if: 1\n"); */
    next_shift_start = schedule[shift].shift_start;
/*     printf("prev shift end:%f\n", schedule[shift].shift_end);
    printf("temp_shift_holder: %d\n", shift); */

      /*Check for if previous_shift_end ends after midnight*/
      /*Does not check if it is comparing itself with the employees first shift - this might not be implemented - because prototype.*/
/*     printf("%s: 24 - %f + %f\n", employee.name, previous_shift_end, absentee_shift_start);
 */ if ((24 - absentee_shift_end + next_shift_start) < 11) {
      return false;
    }
  }
/*   printf("next_shift: %d\n", shift);
 */
/*   printf("next shif tstart:%f\n", schedule[shift].shift_start);
 */

/*   printf("%s: 24 - %f + %f\n", employee.name, absentee_shift_end, next_shift_start);
 */  

  temp_shift_holder = check_if_employee_has_shift_this_day(employee, schedule, shift, &off_set, day, month, "today") - off_set;
  
  /*If shift was found tomorrow,
  * checks whether employee has at least 11 hour rest between that starting,
  * and shift on specified day ending*/
  if (temp_shift_holder == SHIFT_NOT_FOUND){
/*     printf("inside if: 2\n");
 */ return true;
  }
  else {
    shift = temp_shift_holder;
    next_shift_start = schedule[shift].shift_start;
    previous_shift_end = schedule[shift].shift_end;
    if ((next_shift_start - absentee_shift_end) < 11 && (absentee_shift_start - previous_shift_end) < 11) {
      return false;
    }
  }
  return true;
}

int check_for_48_hour_rule(employee_s employee){

  return true;
}

int check_for_weekly_day_off(employee_s employee){
  
  return true;
}

/*typedef struct schedule_s {
  char weekday[MAX_WEEK_DAY_NAME];
  int day;
  int month;
  double shift_start;
  double shift_end;
  char employee_name[MAX_STRING_LENGTH];
  char shift_position[MAX_STRING_LENGTH];
  int youth_worker;

} schedule_s;*/