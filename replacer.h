#define MAX_POSITIONS 100
#define MAX_LINE_LENGTH 500
#define MAX_STRING_LENGTH 100
#define MAX_PHONE 9

typedef struct employee_s {
  char name[MAX_STRING_LENGTH];
  int youth_worker;         /* Boolean */
  int weekday_availability; /* Boolean */
  char phone_number[MAX_PHONE];
  int number_of_positions; /* Used internally */
  char positions[MAX_POSITIONS][MAX_STRING_LENGTH];
} employee_s;
