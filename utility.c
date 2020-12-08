#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function prototypes */
int count_elements(FILE *fp);
void capitalize_string(char *str);
void str_mem_alloc_check(char *dynamic_array);
void file_open_check(FILE *file_pointer);
int display_screen(char *print_list[], int size);

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
 * manageable. The output parameter is an integer, representing the user's
 * choice.
 *
 * @param print_list The array of strings that will be displayed in the
 * "window".
 * @return int An integer, representing the user choice, based on the dialog.
 */
int display_screen(char *print_list[], int size) {
  int i, ch, menu_choice;

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
  for (i = 0; i < size; i++) {
    printf("| %-76s |\n", print_list[i]);
  }
  printf("| %-76s |\n"
         "+--------------------------------------------------------------------"
         "----------+\n\n"
         "Choice: ",
         "");
  scanf(" %d", &menu_choice);
  while ((ch = getchar()) != '\n' && ch != EOF)
    continue;
  return menu_choice;
}
