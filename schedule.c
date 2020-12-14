#include "schedule.h"
#include "employee.h"
#include "rules.h"
#include "utility.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void find_replacement(employee_s employees[], int num_of_employees,
                      int num_of_total_positions);
void fill_schedule_with_data(schedule_s schedule[], FILE *schedule_fp,
                             int shifts);
void edit_schedule(schedule_s schedule[], FILE *schedule_fp,
                   int number_of_shifts, employee_s employees[],
                   int num_of_employees, int num_of_total_positions);
bool check_if_possible_replacements(employee_s possible_replacements[],
                                    int num_of_employees,
                                    char scanned_employee_name[]);

void store_schedule_file(schedule_s schedule[], FILE *schedule_fp,
                         int number_of_shifts);

/**
 * @brief Starts process of picking out who is absent and checking who can
 * replace them. Also changes schedule.csv file.
 *
 * @param employees array of structs of employees.
 * @param num_of_employees number of employees in employees.csv file.
 * @param num_of_total_positions number of positions in positions.txt file.
 */
void find_replacement(employee_s employees[], int num_of_employees,
                      int num_of_total_positions) {
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
                             int number_of_shifts) {
  int i;
  char input_string[MAX_LINE_LENGTH];
  fseek(schedule_fp, 0, SEEK_SET);

  for (i = 0; i < number_of_shifts; i++) {
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
 * @param num_of_total_positions number of positions in positions.txt file.
 */
void edit_schedule(schedule_s schedule[], FILE *schedule_fp,
                   int number_of_shifts, employee_s employees[],
                   int num_of_employees, int num_of_total_positions) {
  int i, j, shift, day, month;
  bool replacement_exists_with_wrong_qualifications =
      false;           /*Checks if a possible replacement exists with wrong
                          qualifactions*/
  bool employee_found; /*Bool to determine whether employee with input name
                          exist*/
  char name_of_absent_employee[MAX_STRING_LENGTH],
      temp_string[MAX_STRING_LENGTH];
  employee_s *possible_replacements; /*array of possible replacements for
                                        absentee's shift*/
  time_t now = time(NULL);           /*Struct which gets current system time*/
  struct tm *system_time; /*time.h importet struct for day & month of local
                             system time*/

  do {
    possible_replacements =
        (employee_s *)calloc(num_of_employees, sizeof(employee_s));
    employee_mem_alloc_check(possible_replacements);
    shift = -1, i = 0, day = 0, month = 0, employee_found = true;

    do {
      clear_screen();
      printf("\nIS THE ABSENT EMPLOYEE'S SHIFT TODAY? (YES/NO)\n");
      scanf(" %s", temp_string);
      fflush(stdin); /* Used to clear the input buffer */
      capitalize_string(temp_string);
      if (!strcmp(temp_string, "YES")) {
        /*Gets system time and assigns day and month. From:
         * https://cboard.cprogramming.com/c-programming/62595-how-get-system-date.html*/
        system_time = localtime(&now);
        month = system_time->tm_mon + 1;
        day = system_time->tm_mday;
        printf("SYSTEM DATE TODAY: %d/%d\n", day, month);
        break;
      }
      clear_screen();
    } while (strcmp(temp_string, "NO") && strcmp(temp_string, "YES"));

    if (day == 0 && month == 0) {
      do {
        /*User picks date for absentee employee*/
        printf("\nENTER DAY AND MONTH FOR ABSENT EMPLOYEE'S SHIFT\n"
               "TYPE '0/0' TO QUIT'\n");
        scanf(" %d/%d", &day, &month);
        fflush(stdin);
        clear_screen();
      } while (
          day > 31 || month > 12 || day < 0 ||
          month <
              0); /*Tomorrow to check for days not existing? - day and month
                      should exit loop if 0 so can break in next while loop*/
    }
    if (day == 0 && month == 0) {
      free(possible_replacements);
      continue;
    }

    /*Goes through all shifts until it finds one this day & month,
    then it prints all shifts with the inputted day and month*/
    do {
      shift++;
      if (schedule[shift].day == day && schedule[shift].month == month) {
        clear_screen();
        printf("\nDATE: %s %d/%d\n", schedule[shift].weekday,
               schedule[shift].day, schedule[shift].month);
      }
      while (schedule[shift].day == day && schedule[shift].month == month) {
        printf("EMPLOYEE: %-20s TIME: %5.2f-%5.2f. ROLE: %s\n",
               schedule[shift].employee_name, schedule[shift].shift_start,
               schedule[shift].shift_end, schedule[shift].shift_position);
        i++;
        shift++;
      }
      if (i > 0 || shift == number_of_shifts) {
        break;
      }
    } while (schedule[shift].day != day || schedule[shift].month != month);

    if (i == 0) {
      printf("\nERROR. NO SHIFTS FOUND ON SPECIFIED DATE: %d/%d\n\n", day,
             month);
      wait_time(3);
      clear_screen();
      free(possible_replacements);
      continue;
    }
    shift -= i; /*To reset shift back to the first shift on the specified date,
    shift is subtracted with a value equal to what it was incremented with in
    the previous loop*/

    do {
      i = 0;
      if (employee_found == false) {
        printf("\n\nERROR. NO SHIFTS FOUND FOR %s ON SPECIFIED DATE: %d/%d\n\n",
               name_of_absent_employee, day, month);
        wait_time(3);
      }
      employee_found = false;
      /*Gets user input as to what shift to change*/
      printf("\nENTER NAME OF ABSENT EMPLOYEE.\n");
      scanf(" %s", name_of_absent_employee);
      fflush(stdin);
      capitalize_string(name_of_absent_employee);
      while (schedule[shift].day == day && schedule[shift].month == month) {
        if (!strcmp(schedule[shift].employee_name, name_of_absent_employee)) {
          clear_screen();
          printf("\nCHOSEN SHIFT:\n%s DATE: %s %d/%d TIME: %5.2f-%5.2f. ROLE: "
                 "%s\n\n",
                 schedule[shift].employee_name, schedule[shift].weekday,
                 schedule[shift].day, schedule[shift].month,
                 schedule[shift].shift_start, schedule[shift].shift_end,
                 schedule[shift].shift_position);
          employee_found = true; /*Check to make sure that shift for specified
                                    employee was found*/
          break;
        }
        shift++;
        i++;
      }
      if (employee_found == true) {
        break;
      }
      shift -= i; /*Resets shift to first shift on specified date again*/
    } while (i != 0);

    j = 0;

    /*Checks which employees do not breach legislation if they were assigned the
     * shift and puts these into an array.*/
    for (i = 0; i < num_of_employees; i++) {
      if (check_for_rules(&employees[i], schedule, shift, number_of_shifts, day,
                          month)) {
        possible_replacements[j] = employees[i];
        possible_replacements[j].points = 0;
        possible_replacements[j].is_qualified = false;
        j++;
      }
    }

    for (i = 0; i < j; i++) {
      check_for_weekday_availability(&possible_replacements[i],
                                     schedule[shift]);
      check_for_youth_worker(&possible_replacements[i], schedule[shift]);
      check_for_qualifications(&possible_replacements[i], schedule[shift],
                               num_of_total_positions);
    }
    sort_replacements(possible_replacements, j);

    j = 0;
    printf("\nPOSSIBLE REPLACEMENTS ARE:\n"
           "%-20s %15s %24s\n\n",
           "REPLACEMENT SCORE", "EMPLOYEE", "PHONE NUMBER");
    while (possible_replacements[j].name[0] != '\0') {
      if (possible_replacements[j].is_qualified) {
        printf("%8d %18s %-15s%16s\n", possible_replacements[j].points, " ",
               possible_replacements[j].name,
               possible_replacements[j].phone_number);
      } else {
        replacement_exists_with_wrong_qualifications = true;
      }
      j++;
    }
    if (replacement_exists_with_wrong_qualifications) {
      j = 0;
      printf("\n\nPOSSIBLE REPLACEMENTS WITHOUT CORRECT QUALIFICATIONS ARE:\n"
             "%-20s %15s %24s\n\n",
             "REPLACEMENT SCORE", "EMPLOYEE", "PHONE NUMBER");
      while (possible_replacements[j].name[0] != '\0') {
        if (!possible_replacements[j].is_qualified) {
          printf("%8d %18s %-15s%16s\n", possible_replacements[j].points, " ",
                 possible_replacements[j].name,
                 possible_replacements[j].phone_number);
        }
        j++;
      }
    }

    /*Gets user input of which employee is going to cover shift.
     * Checks if employer is in legal replacement array.*/
    do {
      printf("\nENTER NAME OF REPLACEMENT.\nTO SELECT ANOTHER SHIFT\n"
             "TYPE 'CANCEL'\n");
      scanf(" %s", temp_string);
      fflush(stdin); /* Used to clear the input buffer */
      capitalize_string(temp_string);
    } while (!check_if_possible_replacements(possible_replacements,
                                             num_of_employees, temp_string));
    free(possible_replacements);
    if (!strcmp(temp_string, "CANCEL")) {
      clear_screen();
      continue;
    }
    strcpy(schedule[shift].employee_name, temp_string);

    printf("\nREPLACEMENT COMPLETE\n");
    wait_time(3);
    clear_screen();
    break;
    /*Loops until user has done all desired changes*/
  } while (day != 0 && month != 0);

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
 * @return bool. Whether employee is legal replacement or not.
 */
bool check_if_possible_replacements(employee_s possible_replacements[],
                                    int num_of_employees,
                                    char scanned_employee_name[]) {

  int i;
  if (!strcmp(scanned_employee_name, "CANCEL")) {
    return true;
  }
  for (i = 0; i <= num_of_employees; i++) {
    if (!strcmp(scanned_employee_name, possible_replacements[i].name)) {
      return true;
    }
  }
  printf("\nEMPLOYEE SELECTED NOT LEGAL REPLACEMENT\n");
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
                         int number_of_shifts) {
  int i;
  fseek(schedule_fp, 0, SEEK_SET);
  for (i = 0; i < number_of_shifts; i++) {
    fprintf(schedule_fp, "%s,%.2d,%.2d,%4.2f,%4.2f,%s,%s,%d\n",
            schedule[i].weekday, schedule[i].day, schedule[i].month,
            schedule[i].shift_start, schedule[i].shift_end,
            schedule[i].employee_name, schedule[i].shift_position,
            schedule[i].youth_worker);
  }
}
