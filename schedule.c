#include "schedule.h"
#include "employee.h"
#include "rules.h"
#include "utility.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void find_replacement(employee_s employees[], int num_of_employees, int num_of_total_positions);
void fill_schedule_with_data(schedule_s schedule[], FILE *schedule_fp,
                             int shifts);
void edit_schedule(schedule_s schedule[], FILE *schedule_fp,
                   int number_of_shifts, employee_s employees[],
                   int num_of_employees, int num_of_total_positions);
int check_if_possible_replacements(employee_s possible_replacements[], int num_of_employees,
                                   char scanned_employee_name[]);

void store_schedule_file(schedule_s schedule[], FILE *schedule_fp,
                         int number_of_shifts);

/**
 * @brief Starts process of picking out who is absent and checking who can
 * replace them. Also changes schedule.csv file.
 *
 * @param employees array of structs of employees.
 * @param num_of_employees number of employees in employees.csv file.
 */
void find_replacement(employee_s employees[], int num_of_employees, int num_of_total_positions)
{
  int number_of_shifts;
  schedule_s *schedule =
      (schedule_s *)calloc(MAX_NUMBER_OF_SHIFTS, sizeof(schedule_s));
  FILE *schedule_fp = fopen("schedule.csv", "r");
  schedule_mem_alloc_check(schedule);
  file_open_check(schedule_fp);

  number_of_shifts = count_elements(schedule_fp);
  schedule =
      (schedule_s *)realloc(schedule, sizeof(schedule_s) * number_of_shifts);
  schedule_mem_alloc_check(schedule);

  fill_schedule_with_data(schedule, schedule_fp, number_of_shifts);

  fclose(schedule_fp);

  edit_schedule(schedule, schedule_fp, number_of_shifts, employees,
                num_of_employees, num_of_total_positions);
}

/**
 * @brief Refills schedule.csv with new data aquired from reading and changing
 * data.
 *
 * @param schedule array of structs of shifts.
 * @param schedule_fp filepointer to schedule.csv file.
 * @param number_of_shifts number of shifts in schedule.csv file.
 */
void fill_schedule_with_data(schedule_s schedule[], FILE *schedule_fp,
                             int number_of_shifts)
{
  int i;
  char input_string[MAX_LINE_LENGTH];
  fseek(schedule_fp, 0, SEEK_SET);

  for (i = 0; i < number_of_shifts; i++)
  {
    fgets(input_string, MAX_LINE_LENGTH, schedule_fp);
    /*Gets the data contained in schedule.csv and fills array of structs of
     * shifts with it*/
    sscanf(input_string, "%[^,],%d,%d,%lf,%lf,%[^,],%[^,],%d\n",
           schedule[i].weekday, &schedule[i].day, &schedule[i].month,
           &schedule[i].shift_start, &schedule[i].shift_end,
           schedule[i].employee_name, schedule[i].shift_position,
           &schedule[i].youth_worker);
    schedule[i].weekday[MAX_WEEK_DAY_NAME - 1] = '\0';
    schedule[i].employee_name[MAX_STRING_LENGTH - 1] = '\0';
    schedule[i].shift_position[MAX_STRING_LENGTH - 1] = '\0';
  }
}
/**
 * @brief Gets input from user to find absentee's shift, check for possible
 * replacements, gets user to pick replacement and edits the schedue.csv file
 *
 * @param schedule array of structs of shifts.
 * @param schedule_fp filepointer to schedule.csv file.
 * @param number_of_shifts number of shifts in schedule.csv file.
 * @param employees array of structs of employees.
 * @param num_of_employees number of employees in employees.csv file.
 */
void edit_schedule(schedule_s schedule[], FILE *schedule_fp,
                   int number_of_shifts, employee_s employees[],
                   int num_of_employees, int num_of_total_positions) {
  int i, j = 0, k, shift = -1, day = 0, month = 0;
  bool any_employee_has_no_points = false;
  char name_of_absent_employee[MAX_STRING_LENGTH], temp_string[MAX_STRING_LENGTH], 
       display_choice[MAX_DISPLAY_ELEMENTS][MAX_STRING_LENGTH],
       display_choice_error_messages[MAX_DISPLAY_ELEMENTS][MAX_STRING_LENGTH];
  employee_s *possible_replacements;

  do
  {
    possible_replacements = (employee_s *)calloc(num_of_employees, sizeof(employee_s));
    employee_mem_alloc_check(possible_replacements);
    shift = -1;
    i = 0;
    j = 1;
    do
    {
      /*User picks date for absentee employee ((((IMPROVE: use time.h to find
       * current date as option.))))*/
      sprintf(display_choice[k++],"ENTER DAY AND MONTH FOR ABSENT EMPLOYEE'S SHIFT");
      sprintf(display_choice[k], "TYPE '0/0 TO QUIT'");
      display_screen(display_choice, k);
      scanf(" %d/%d", &day, &month);
    } while (day > 31 || month > 12 || day < 0 ||
             month <
                 0); /*Tomorrow to check for days not existing? - day and month
                        should exit loop if 0 so can break in next while loop*/
    fflush(stdin);   /* Used to clear the input buffer */
    if (day == 0 && month == 0)
    {
      free(possible_replacements);
      continue;
    }

    k = 0;
    /*Goes through all shifts until it finds one this day & month,
    then it prints all shifts with the inputted day and month*/
    do
    {
      shift++;
      while (schedule[shift].day == day && schedule[shift].month == month) {
        sprintf(display_choice[k++],"EMPLOYEE: %-20s TIME: %5.2f-%5.2f. ROLE: %s",
               schedule[shift].employee_name, schedule[shift].shift_start,
               schedule[shift].shift_end, schedule[shift].shift_position);
        i++;
        shift++;
      }
      if (i > 0) {
        break;
      }
    } while (schedule[shift].day != day || schedule[shift].month != month);
    if (i == 0) {
      sprintf(display_choice_error_messages[0],"ERROR. NO SHIFTS FOUND ON SPECIFIED DATE: %d/%d", day, month);
      display_screen(display_choice_error_messages, 0);
      wait_time(3);
    }
    shift -= i;

      do {
        i = 0;
        if (j == 0) {
          sprintf(display_choice_error_messages[0],"ERROR. NO SHIFTS FOUND FOR %s ON SPECIFIED DATE: %d/%d", name_of_absent_employee, day, month);
          display_screen(display_choice_error_messages, 0);
          wait_time(3);
        }
        j = 0; 
        /*Gets user input as to what shift to change*/
        sprintf(display_choice[k++],"");
        sprintf(display_choice[k],"ENTER NAME OF ABSENT EMPLOYEE.");
        display_screen(display_choice, k);
        scanf(" %s", name_of_absent_employee);
        capitalize_string(name_of_absent_employee);
       
        while (schedule[shift].day == day && schedule[shift].month == month) {
          if (!strcmp(schedule[shift].employee_name, name_of_absent_employee)) {
            k = 0;
            sprintf(display_choice[k++], "CHOSEN SHIFT:");
            sprintf(display_choice[k++],"%s TIME: %5.2f-%5.2f. ROLE: %s",
                  schedule[shift].employee_name, schedule[shift].shift_start,
                  schedule[shift].shift_end, schedule[shift].shift_position);
            sprintf(display_choice[k++], "");
            j = 1;
            break;
          }
          shift++;
          i++;
        }
      if (j == 1) {
        break;
      }
      shift -= i;
    } while (i != 0);

    j = 0;
    /*Checks which employees do not breach legislation if they took the shift,
     * and prints them together with phone number.*/

    for (i = 0; i < num_of_employees; i++) {
      if (check_for_rules(&employees[i], schedule, shift, number_of_shifts, day, month)) {
        possible_replacements[j] = employees[i];
        possible_replacements[j].points = 0;
        j++;
      }
    }
    /*lav for loop med de næste 3 funktioner i sig - kræver ændring i funktionerne*/
    check_for_weekday_availability(possible_replacements, j, schedule[shift]);

    check_for_youth_worker(possible_replacements, j, schedule[shift]);

    check_for_qualifications(possible_replacements, j, schedule[shift], num_of_total_positions);

    sort_replacements(possible_replacements, j);

    j = 0;
    sprintf(display_choice[k++],"POSSIBLE REPLACEMENTS ARE:");
    while (possible_replacements[j].name[0] != '\0') {
      if (possible_replacements[j].points > 0) {
        sprintf(display_choice[k++],"REPLACEMENT SCORE: %-15d %-30s%s", possible_replacements[j].points, possible_replacements[j].name, possible_replacements[j].phone_number);
      } 
      else {
      any_employee_has_no_points = true;
      }
      j++;
    }
    if (any_employee_has_no_points) {
      j = 0;
      sprintf(display_choice[k++],"POSSIBLE REPLACEMENTS WITHOUT CORRECT QUALIFICATIONS ARE:");
      while (possible_replacements[j].name[0] != '\0') {
        if (possible_replacements[j].points == 0) {
          sprintf(display_choice[k++],"%-30s%s\n", possible_replacements[j].name, possible_replacements[j].phone_number);
        }
        j++;
      }
    }
  

      /*To do:*/
      /*call check_for_rules to check all who is legally able to work*/
      /*Checking if the replacement is also a youth worker & available in day
       * time also needs implementation*/
    /*call function which contains check_for_rules, and also contains youth
       * worker check, qualificaitons check and day time check*/

    /*Here the employer/manager calls the employee they want to cover shift.*/

    /*Gets user input of which employee is going to cover shift. Checks if
     * employer exists in array.*/
    do {
      sprintf(display_choice[k++],"");
      sprintf(display_choice[k++],"ENTER NAME OF REPLACEMENT.");
      sprintf(display_choice[k],"TO SELECT ANOTHER SHIFT TYPE 'change'");
      display_screen(display_choice, k);
      k -= 3;
      scanf(" %s", temp_string);
      capitalize_string(temp_string);
      fflush(stdin); /* Used to clear the input buffer */
    } while (!check_if_possible_replacements(possible_replacements, num_of_employees,
                                             temp_string));
    free(possible_replacements);
    if (!strcmp(temp_string, "CHANGE")) {
      continue;
    }
    strcpy(schedule[shift].employee_name, temp_string);
    /*Skal løbe array igennem over possible_replacements, det indeholder KUN
     * legal replacements, er medarbejder ikke er så fortæl user.*/
   
    
    sprintf(display_choice[0], "REPLACEMENT COMPLETE:");
    display_screen(display_choice, 0);
    wait_time(3);
    day = 0;
    month = 0;
    /*Loops until user has done all desired changes*/
  } while (day != 0 && month != 0);

  /*Opens schedule.csv in write-mode.*/
  schedule_fp = fopen("schedule.csv", "w");
  file_open_check(schedule_fp);

  /*Prints new arrays of struct into schedule.csv, closes the file and then
   * returns. This is done after all changes have been made*/
  store_schedule_file(schedule, schedule_fp, number_of_shifts);
  fclose(schedule_fp);
  free(schedule);
}

/**
 * @brief Checks if employee exists in array of employees already, otherwise
 * returns false.
 *
 * @param employees array of structs of employees.
 * @param num_of_employees number of employees in employees.csv file.
 * @param scanned_employee_name name of the user inputted employee name to check
 * if exists.
 * @return int
 */
int check_if_possible_replacements(employee_s possible_replacements[], int num_of_employees,
                             char scanned_employee_name[]) {
  
  char display_choice[][MAX_STRING_LENGTH] = {"EMPLOYEE SELECTED NOT LEGAL REPLACEMENT"};
  int i;
  if (!strcmp(scanned_employee_name, "CHANGE")) {
    return true;
  }
  for (i = 0; i <= num_of_employees; i++) {
    if (!strcmp(scanned_employee_name, possible_replacements[i].name)) {
      return true;
    }
  }
  display_screen(display_choice, 0);
  wait_time(3);
  return false;
}

/**
 * @brief Re-fills schedule.csv file with the data changed after finding a
 * replacement
 *
 * @param schedule array of structs of shifts.
 * @param schedule_fp filepointer to schedule.csv file.
 * @param number_of_shifts number of shifts in schedule.csv file.
 */
void store_schedule_file(schedule_s schedule[], FILE *schedule_fp,
                         int number_of_shifts)
{
  int i;
  fseek(schedule_fp, 0, SEEK_SET);
  for (i = 0; i < number_of_shifts; i++)
  {
    fprintf(schedule_fp, "%s,%.2d,%.2d,%4.2f,%4.2f,%s,%s,%d\n", schedule[i].weekday,
            schedule[i].day, schedule[i].month, schedule[i].shift_start,
            schedule[i].shift_end, schedule[i].employee_name,
            schedule[i].shift_position, schedule[i].youth_worker);
  }
}

/*void print_schedule(schedule_s schedule[], int number_of_shifts) {
  int i, j, start_day, start_month, days_forward;
  printf("Enter the start date and end date you wish to see (dd/mm days):\n");
  scanf("%d/%d %d", &start_day, &start_month, &days_forward);
  for (i = 0; i < 100; i++) {
    if (start_day == schedule[i].day && start_month == schedule[i].month) {
      printf("%d %-9s %.2d/%.2d %.4d-%.4d %-10s %s \n", schedule[i].weekday,
             schedule[i].day, schedule[i].month, schedule[i].shift_start,
             schedule[i].shift_end, schedule[i].employee_name,
             schedule[i].shift_position);
    }
  }
} */
