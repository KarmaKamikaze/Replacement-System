#include "schedule.h"
#include "employee.h"
#include "rules.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void find_replacement(employee_s employees[], int num_of_employees);
void fill_schedule_with_data(schedule_s schedule[], FILE *schedule_fp,
                             int shifts);
void edit_schedule(schedule_s schedule[], FILE *schedule_fp,
                   int number_of_shifts, employee_s employees[],
                   int num_of_employees);
int check_if_employee_exists(employee_s employees[], int num_of_employees,
                             char scanned_employee_name[]);

void store_schedule_file(schedule_s schedule[], FILE *schedule_fp,
                          int number_of_shifts);

/**
 * @brief Starts process of picking out who is absent and checking who can replace them. Also changes schedule.csv file.
 * 
 * @param employees array of structs of employees.
 * @param num_of_employees number of employees in employees.csv file.
 */
void find_replacement(employee_s employees[], int num_of_employees) {
  schedule_s schedule[DAYS_IN_SCHEDULE];
  int number_of_shifts;
  FILE *schedule_fp = fopen("schedule.csv", "r");
  if (schedule_fp == NULL) {
    perror("Error");
    exit(EXIT_FAILURE);
  }
  number_of_shifts = count_elements(schedule_fp);
  fill_schedule_with_data(schedule, schedule_fp, number_of_shifts);

  fclose(schedule_fp);

  edit_schedule(schedule, schedule_fp, number_of_shifts, employees,
                num_of_employees);
}

/**
 * @brief Refills schedule.csv with new data aquired from reading and changing data.
 * 
 * @param schedule array of structs of shifts.
 * @param schedule_fp filepointer to schedule.csv file.
 * @param number_of_shifts number of shifts in schedule.csv file.
 */
void fill_schedule_with_data(schedule_s schedule[], FILE *schedule_fp,
                             int number_of_shifts) {
  int i;
  char input_string[MAX_STRING_LENGTH];
  fseek(schedule_fp, 0, SEEK_SET);
  for (i = 0; i < number_of_shifts; i++) {
    fgets(input_string, MAX_STRING_LENGTH, schedule_fp);
    /*Gets the data contained in schedule.csv and fills array of structs of shifts with it*/
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
 * @brief Gets input from user to find absentee's shift, check for possible replacements, gets user to pick replacement and edits the schedue.csv file
 * 
 * @param schedule array of structs of shifts.
 * @param schedule_fp filepointer to schedule.csv file.
 * @param number_of_shifts number of shifts in schedule.csv file.
 * @param employees array of structs of employees.
 * @param num_of_employees number of employees in employees.csv file.
 */
void edit_schedule(schedule_s schedule[], FILE *schedule_fp,
                   int number_of_shifts, employee_s employees[],
                   int num_of_employees) {
  int i, j = 0, shift, ch;
  int day = 0, month = 0;
  char name_of_absent_employee[MAX_STRING_LENGTH], possible_replacements[MAX_STRING_LENGTH];  /*!!!!!!!!!!!!!!!!SKAL ÆNDRES TIL CALLOC!!!!!!!!!!!!!!!!*/

  do {
    shift = - 1;
    i = 0;
    do {
      /*User picks date for absentee employee ((((IMPROVE: use time.h to find current date as option.))))*/
      printf("Enter day and month for the shift to find replacement for. (Alternatively type '0/0' to quit)\n");
      scanf(" %d/%d", &day, &month);
    } while (day > 31 || month > 12 || day < 0 || month < 0); /*Tomorrow to check for days not existing? - day and month should exit loop if 0 so can break in next while loop*/
    while ((ch = getchar()) != '\n' && ch != EOF);
    if (day == 0 && month == 0) {
      continue;
    }

    /*Goes through all shifts until it finds one this day & month,
    then it prints all shifts with the inputted day and month*/
    do {
      shift++;
      while (schedule[shift].day == day && schedule[shift].month == month) {
        printf("Employee: %-20s Time: %5.2f-%5.2f. Role: %s\n",schedule[shift].employee_name, schedule[shift].shift_start, schedule[shift].shift_end, schedule[shift].shift_position);
        i++;
        shift++;
      }
      if (i > 0) {
        break;
      }
    } while (schedule[shift].day != day || schedule[shift].month != month);

    if (i == 0) {
      printf("No shifts on date: %d/%d\n", day, month);
    }
    shift -= i;

    /*Gets user input as to what shift to change*/
    printf("Enter name of the person who is sick.\n");
    scanf(" %s", name_of_absent_employee);
    while (schedule[shift].day == day && schedule[shift].month == month) {
      if (strcmp(schedule[shift].employee_name, name_of_absent_employee) == 0) {
        printf("Chosen shift:\n%s Time: %5.2f-%5.2f. Role: %s\n\n", schedule[shift].employee_name, schedule[i].shift_start, schedule[i].shift_end);
        break;
      }
      shift++;
    }
  
  /*Checks which employees do not breach legislation if they took the shift, and prints them together with phone number.*/
  for (i = 0; i < num_of_employees; i++) {
    if (check_for_rules(employees[i], schedule, shift, day, month)) {
      possible_replacements[j];
      j++;
    }
    /*To do:*/
    /*call check_for_rules to check all who is legally able to work*/
    /*Checking if the replacement is also a youth worker & available in day time also needs implementation*/
    /*call function which contains check_for_rules, and also contains youth worker check, qualificaitons check and day time check*/
  }

  /*Here the employer/manager calls the employee they want to cover shift.*/

  /*Gets user input of which employee is going to cover shift. Checks if employer exists in array.*/
  do {
    printf("Enter name of employee covering shift.\nTo select another shift type 'change'\n");
    scanf(" %s", schedule[shift].employee_name);
    while ((ch = getchar()) != '\n' && ch != EOF);
  } while (!check_if_employee_exists(employees, num_of_employees, schedule[shift].employee_name)); 
  if (!strcmp(schedule[shift].employee_name,"change")) {
    continue;
  }
  /*!!!!!OVENSTÅENDE SKAL ÆNDRES TIL CHECK_IF_LEGAL_EMPLOYEE!!!!!!!!!*/
  /*Skal løbe array igennem over possible_replacements, det indeholder KUN legal replacements, er medarbejder ikke er så fortæl user.*/


  /*Loops until user is has done all desired changes*/
  } while (day != 0 && month != 0);

  /*Opens schedule.csv in write-mode.*/
  schedule_fp = fopen("schedule.csv", "w");
  if (schedule_fp == NULL) {
    perror("Error");
    exit(EXIT_FAILURE);
  }
  /*Prints new arrays of struct into schedule.csv, closes the file and then returns.
  * This is done after all changes have been made*/
  store_schedule_file(schedule, schedule_fp, number_of_shifts);
  fclose(schedule_fp);
  
}

/**
 * @brief Checks if employee exists in array of employees already, otherwise returns false.
 * 
 * @param employees array of structs of employees.
 * @param num_of_employees number of employees in employees.csv file.
 * @param scanned_employee_name name of the user inputted employee name to check if exists.
 * @return int 
 */
int check_if_employee_exists(employee_s employees[], int num_of_employees,
                             char scanned_employee_name[]) {


  int i, found_employee_bool = 0;
  if (!strcmp(scanned_employee_name, "change")) {
    return true;
  }
  capitalize_string(scanned_employee_name);
  for (i = 0; i <= num_of_employees; i++) {
    if (!strcmp(scanned_employee_name,
                employees[i].name)) {
      found_employee_bool = 1;
      break;
    }
  }
  printf("EMPLOYEE NOT FOUND.\n");
  return false;
}

/**
 * @brief Re-fills schedule.csv file with the data changed after finding a replacement
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
    fprintf(schedule_fp, "%s,%d,%d,%.2f,%.2f,%s,%s,%d\n", schedule[i].weekday,
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
