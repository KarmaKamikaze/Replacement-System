#include <stdio.h>

/* ~~ SYMBOLIC CONSTANTS ~~ */
#define MAX_WEEK_DAY_NAME 10
#define DAYS_IN_SCHEDULE  120
#define MAX_LINE_LENGTH 500
#define MAX_STRING_LENGTH 100

/* ~~ STRUCTS ~~ */
typedef struct schedule_s {
  int day_of_year; /* Used to track which day it is. */
  char weekday[MAX_WEEK_DAY_NAME];
  int day;
  int month;
  double shift_start; /* 0100, 1630, etc. */
  double shift_end;
  char employee_name[MAX_STRING_LENGTH];
  char shift_position[MAX_STRING_LENGTH];

} schedule_s;

/* ~~ PROTOTYPES ~~ */
void schedule_function (void);
void fill_schedule_with_data (schedule_s schedule[], FILE *schedule_fp, int shifts);
int count_shifts (FILE *schedule_fp);
void print_schedule (schedule_s schedule[], int shifts);
