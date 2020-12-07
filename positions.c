#include "employee.h"
#include "utility.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Function protoypes */
int parse_positions(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH]);
void store_positions(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                     int num_of_total_positions);
void new_position(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                  int *num_of_total_positions);
void delete_position(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                     int *num_of_total_positions);

/**
 * @brief This function reads the existing positions from a file, if such a file
 * exists. If it doesn't, it creates one. The positions are added to the string
 * array for the program to use.
 *
 * @param positions_str_arr Output parameter. A string array, which will contain
 * the existing positions, if the file exists.
 * @return int The number of existing positions.
 */
int parse_positions(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH]) {
  FILE *fp;
  int i, num_of_total_positions;
  char input_string[MAX_STRING_LENGTH];

  do {
    fp = fopen("positions.txt", "r");
    if (fp == NULL) {
      printf("File positions.txt was not found. Creating new file.");
      fp = fopen("positions.txt", "w");
      fclose(fp);
    }
  } while (fp == NULL);

  num_of_total_positions = count_elements(fp);

  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < num_of_total_positions; i++) {
    fgets(input_string, MAX_STRING_LENGTH, fp);
    strcpy(positions_str_arr[i], input_string);
  }

  fclose(fp);

  return num_of_total_positions;
}

/**
 * @brief This function saves the positions currently in the position string
 * array to the positions.txt file. This ensures that this data is kept between
 * runtimes.
 *
 * @param positions_str_arr A string array containing the existing positions
 * that we wish to store in a file.
 * @param num_of_total_positions The number of strings, containing positions, in
 * the position string array.
 */
void store_positions(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                     int num_of_total_positions) {
  FILE *fp;
  int i;

  fp = fopen("positions.txt", "w");

  for (i = 0; i < num_of_total_positions; i++)
    fprintf(fp, "%s", positions_str_arr[i]);

  /* Check if last positions final character is a newline. If not, print a
   * newline. Also checks if there are any positions.*/
  if (positions_str_arr[num_of_total_positions - 1]
                       [strlen(positions_str_arr[num_of_total_positions - 1]) -
                        1] != '\n' && num_of_total_positions != 0)
    fprintf(fp, "\n");
  fclose(fp);
}

/**
 * @brief This function adds a new position to the string array of already
 * existing positions. Through dialog with the user, they are able to input the
 * name of a new position through the terminal, after which the function
 * determins if it exists already. If it does not, the position will be added to
 * the array.
 *
 * @param positions_str_arr Output parameter. A string array containing already
 * existing positions. The new position will be appended to this array.
 * @param num_of_total_positions Output parameter. The number of current
 * positions. This value will be incremented if a new position is added.
 */
void new_position(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                  int *num_of_total_positions) {
  char temp_string[MAX_STRING_LENGTH], throwaway_string[MAX_STRING_LENGTH];
  bool duplicate_check;
  int i;

  printf("ENTER NEW POSITION: ");
  scanf("%[^\n]", temp_string);
  fgets(throwaway_string, MAX_STRING_LENGTH, stdin);
  strcpy(temp_string, capitalize_string(temp_string));

  for (i = 0; i < *num_of_total_positions; i++) {
    /* Because the existing positions end with a newline, but temp_string does
     * not, we use strncmp to only compare the relevant characters. */
    if (!strncmp(positions_str_arr[i], temp_string, strlen(positions_str_arr[i]) > strlen(temp_string) ? strlen(positions_str_arr[i])-1 : strlen(temp_string))) {
      duplicate_check = true;
      printf("THIS POSITION ALREADY EXISTS!\n");
      break;
    }
  }
  if (duplicate_check == false) {
    strcpy(positions_str_arr[*num_of_total_positions], temp_string);
    printf("POSITION %s HAS BEEN ADDED\n",
           positions_str_arr[*num_of_total_positions]);
    (*num_of_total_positions)++;
  }
}

/**
 * @brief This function deletes a position from the string array containing
 * existing positions if it exists. It deletes the value in the array by
 * overwriting it with the values above it in the index, moving all indexes one
 * step lower.
 *
 * @param positions_str_arr Output parameter. A string array of currently
 * existing positions. When a position has been deleted, the new array is
 * returned to the function call.
 * @param num_of_total_positions Output parameter. The number of existing
 * positions. This number will be decremented when a position is removed from
 * the array.
 */
void delete_position(char positions_str_arr[MAX_POSITIONS][MAX_STRING_LENGTH],
                     int *num_of_total_positions) {
  int position_value = 0; /*Initialised to 0 to avoid bugs when user inputs letters instead of numbers*/
  char throwaway_string[MAX_STRING_LENGTH];
  int i;

  printf("\nEXISTING POSITIONS:\n");
  for (i = 0; i < *num_of_total_positions; i++)
    printf("%d = %s", i + 1, positions_str_arr[i]);

  printf("\nENTER DIGIT CORRESPONDING TO POSITION TO DELETE: ");
  scanf("%d", &position_value);
  fgets(throwaway_string, MAX_STRING_LENGTH, stdin);
  if (position_value >= 1 && position_value <= *num_of_total_positions) {
    printf("POSITION DELETED: %s", positions_str_arr[position_value-1]);
    if (*num_of_total_positions == 1) {
      remove("positions.txt");
    }
    else { 
      for (i = position_value - 1; i < *num_of_total_positions; i++)
        strcpy(positions_str_arr[i], positions_str_arr[i + 1]);
    }
    (*num_of_total_positions)--;
  } 
  else
    printf("THE CHOSEN POSITION DOES NOT EXIST!\n");
}
