#include "utility.h"
#include "employee.h"
#include "schedule.h"

#ifndef RULES_H
#define RULES_H

#define MAX_DETERMINANT_LENGTH 10
#define SHIFT_NOT_FOUND 999

typedef enum switch_determinant_e {yesterday = -1, today, tomorrow};

int check_for_rules(employee_s employee, schedule_s schedule[], int shift, int day, int month);

#endif