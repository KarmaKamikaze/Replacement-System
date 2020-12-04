#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function prototypes */
int count_elements(FILE *fp);
char *capitalize_string(char *str);
int display_screen(char *print_list[]);

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

char *capitalize_string(char *str) {
  int i;
  char *capitalized_string = (char *)calloc(strlen(str), sizeof(char));
  if (capitalized_string == NULL) {
    printf("ERROR ALLOCATING MEMORY!\n");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < strlen(str); i++)
    capitalized_string[i] = toupper(str[i]);
  return capitalized_string;
}

int display_screen(char *print_list[]) {
  int i, ch, menu_choice;

  system("clear");
  /* What isn't pretty in the code will have to be pretty elsewhere */
  printf("+--------------------------------------------------------------------"
         "----------+\n"
         "| ############################ REPLACEMENT SYSTEM "
         "############################ |\n"
         "+--------------------------------------------------------------------"
         "----------+\n"
         "| %-76s |\n",
         "");
  for (i = 0; i <= sizeof(*print_list); i++) {
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
