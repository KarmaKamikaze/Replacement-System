#include "utility.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <time.h>
#else
#include <unistd.h>
#endif

/* Function prototypes */
int count_elements(FILE *fp);
void capitalize_string(char *str);
void str_mem_alloc_check(char *dynamic_array);
void file_open_check(FILE *file_pointer);
void display_screen(char print_list[][MAX_STRING_LENGTH], int size);
void wait_time(unsigned int time);
#ifdef _WIN32
void nsleep(long miliseconds);
#endif

/**
 * @brief This function counts the elements in an employee file.
 *
 * @param fp A FILE pointer to the employee file.
 * @return int The number of elements, e.g. the amount of employees stored in
 * the file.
 */
int count_elements(FILE *fp) {
  int elements = 0, ch = 0;
  /* Makes sure we ALWAYS read from the start of our file. */
  fseek(fp, 0, SEEK_SET);
  while (ch != EOF) {
    ch = fgetc(fp);
    if (ch == '\n') {
      elements++;
    }
  }
  return elements;
}

void capitalize_string(char *str) {
  int i;
  for (i = 0; i < strlen(str); i++)
    str[i] = toupper(str[i]);
}

/*IF THIS FUNCTION IS NOT USED AT ALL, DELETE IT*/
void str_mem_alloc_check(char *dynamic_array) {
  if (dynamic_array == NULL) {
    perror("ERROR");
    exit(EXIT_FAILURE);
  }
}

void file_open_check(FILE *file_pointer) {
  if (file_pointer == NULL) {
    perror("ERROR");
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief This function takes an array of strings and displays them in a
 * "window". It will make the dialog, that the user will act upon, look more
 * manageable.
 *
 * @param print_list The array of strings that will be displayed in the
 * "window".
 */
void display_screen(char print_list[][MAX_STRING_LENGTH], int size) {
  int i;

#ifdef _WIN32
  system("cls");

#elif __APPLE__
  system("clear");

#elif __linux__
  system("clear");

#endif

  /* What isn't pretty in the code will have to be pretty elsewhere */
  printf("+--------------------------------------------------------------------"
         "----------+\n"
         "| ############################ REPLACEMENT SYSTEM "
         "############################ |\n"
         "+--------------------------------------------------------------------"
         "----------+\n"
         "| %-76s |\n",
         "");
  for (i = 0; i <= size; i++) {
    printf("| %-76s |\n", print_list[i]);
  }
  printf("| %-76s |\n"
         "+--------------------------------------------------------------------"
         "----------+\n\n"
         ">> ",
         "");
  fflush(stdout); /* Flush the line buffer for wait function */
}

/**
 * @brief This function will pause the program for three seconds, printing
 * waiting symbols, allowing the user to read dialog.
 *
 * @param time The amount of time, in seconds, to wait for.
 */
void wait_time(unsigned int time) {
#ifdef _WIN32
  time = time / 3;
  printf(".");
  fflush(stdout);
  nsleep(time * 1000);
  printf(".");
  fflush(stdout);
  nsleep(time * 1000);
  printf(".");
  fflush(stdout);
  nsleep(time * 1000);
#else
  time = time / 3;
  printf(".");
  fflush(stdout);
  sleep(time);
  printf(".");
  fflush(stdout);
  sleep(time);
  printf(".");
  fflush(stdout);
  sleep(time);
#endif
}

#ifdef _WIN32
void nsleep(long miliseconds) {
  struct timespec req, rem;

  if (miliseconds > 999) {
    req.tv_sec = (int)(miliseconds / 1000); /* Must be Non-Negative */
    req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) *
                  1000000; /* Must be in range of 0 to 999999999 */
  } else {
    req.tv_sec = 0; /* Must be Non-Negative */
    req.tv_nsec =
        miliseconds * 1000000; /* Must be in range of 0 to 999999999 */
  }
  nanosleep(&req, &rem);
}
#endif
