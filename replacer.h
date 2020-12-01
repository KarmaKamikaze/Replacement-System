#define MAX_EMPLOYEES 40
#define TOTAL_POSITIONS 6

typedef struct employee_s {
  char name[100];
  char phone_number[9];
  int youth_worker; /* Boolean */
  int weekday_availability; /* Boolean */
  int number_of_positions;
  char positions[TOTAL_POSITIONS][20];
} employee_s;