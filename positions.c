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
      printf("\nFile positions.txt was not found. Creating new file.\n");
      wait_time(3);
      clear_screen();
      fp = fopen("positions.txt", "w");
      fclose(fp);
    }
  } while (fp == NULL);

  num_of_total_positions = count_elements(fp);

  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < num_of_total_positions; i++) {
    fgets(input_string, MAX_STRING_LENGTH, fp);
    strcpy(positions_str_arr[i], input_string);
    positions_str_arr[i][strlen(positions_str_arr[i]) - 1] = '\0';
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
    fprintf(fp, "%s\n", positions_str_arr[i]);

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
  char temp_string[MAX_STRING_LENGTH];
  bool duplicate_check;
  int i;

  printf("\nENTER NEW POSITION(TYPE EXIT TO QUIT):\n");
  scanf(" %[^\n]", temp_string);
  fflush(stdin); /* Used to clear the input buffer */
  capitalize_string(temp_string);
  if (strcmp(temp_string, "EXIT")) {
    for (i = 0; i < *num_of_total_positions; i++) {
      /* Because the existing positions end with a newline, but temp_string does
       * not, we use strncmp to only compare the relevant characters. */
      if (!strncmp(positions_str_arr[i], temp_string,
                   strlen(positions_str_arr[i]) > strlen(temp_string)
                       ? strlen(positions_str_arr[i]) - 1
                       : strlen(temp_string))) {
        duplicate_check = true;
        printf("\nTHIS POSITION ALREADY EXISTS!\n");
        break;
      }
    }
    if (duplicate_check == false) {
      strcpy(positions_str_arr[*num_of_total_positions], temp_string);
      printf("\nPOSITION %s HAS BEEN ADDED.\n",
             positions_str_arr[*num_of_total_positions]);
      (*num_of_total_positions)++;
    }
  }
  wait_time(3);
  clear_screen();
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
  int i, position_value = 0; /*Initialised to 0 to avoid bugs when user inputs
                             letters instead of numbers*/

  printf("\nEXISTING POSITIONS:\n\n");
  for (i = 0; i < *num_of_total_positions; i++) {
    printf("%d = %s\n", i + 1, positions_str_arr[i]);
  }
  printf(
      "\nENTER DIGIT CORRESPONDING TO POSITION TO DELETE(TYPE 0 TO EXIT):\n");
  scanf(" %d", &position_value);
  fflush(stdin); /* Used to clear the input buffer */
  if (position_value >= 1 && position_value <= *num_of_total_positions) {
    printf("\nPOSITION DELETED: %s\n", positions_str_arr[position_value - 1]);
    if (*num_of_total_positions == 1) {
      remove("positions.txt");
    } else {
      for (i = position_value - 1; i < *num_of_total_positions; i++)
        strcpy(positions_str_arr[i], positions_str_arr[i + 1]);
    }
    (*num_of_total_positions)--;
  } else {
    if (position_value != 0)
      printf("\nTHE CHOSEN POSITION DOES NOT EXIST!\n");
  }
  wait_time(3);
  clear_screen();
}
