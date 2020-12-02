/* ~~ SYMBOLIC CONSTANTS ~~ */
#define MAX_WEEK_DAY_NAME 10
#define DAYS_IN_SCHEDULE  120
#define MAX_LINE_LENGTH 500
#define MAX_STRING_LENGTH 100

/* ~~ STRUCTS ~~ */
typedef struct schedule_s {
  char weekday[MAX_WEEK_DAY_NAME];
  int day;
  int month;
  int shift_start; /* 0100, 1630, etc. */
  int shift_end;
  char employee_name[MAX_STRING_LENGTH];
  char shift_position[MAX_STRING_LENGTH];

} schedule_s;

/* ~~ PROTOTYPES ~~ */
void schedule_function (void);
FILE *open_schedule_file (FILE *schedule_fp);
void fill_schedule_with_data (schedule_s schedule[], FILE *schedule_fp, int shifts);
int count_shifts (FILE *schedule_fp);
void print_schedule (schedule_s schedule[], int shifts);
