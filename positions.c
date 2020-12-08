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
  char display_choice[][MAX_STRING_LENGTH] = {
      "File positions.txt was not found. Creating new file."};

  do {
    fp = fopen("positions.txt", "r");
    if (fp == NULL) {
      display_screen(display_choice, 0);
      wait(3);
      fp = fopen("positions.txt", "w");
      fclose(fp);
    }
  } while (fp == NULL);

  num_of_total_positions = count_elements(fp);

  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < num_of_total_positions; i++) {
    fgets(input_string, MAX_STRING_LENGTH, fp);
    strcpy(positions_str_arr[i], input_string);
    positions_str_arr[i][strlen(positions_str_arr[i])-1] = '\0';
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
  char temp_string[MAX_STRING_LENGTH], throwaway_string[MAX_STRING_LENGTH];
  char display_choice[][MAX_STRING_LENGTH] = {"ENTER NEW POSITION"};
  bool duplicate_check;
  int i;

  display_screen(display_choice, 0);
  scanf("%[^\n]", temp_string);
  fgets(throwaway_string, MAX_STRING_LENGTH, stdin);
  capitalize_string(temp_string);

  for (i = 0; i < *num_of_total_positions; i++) {
    /* Because the existing positions end with a newline, but temp_string does
     * not, we use strncmp to only compare the relevant characters. */
    if (!strncmp(positions_str_arr[i], temp_string,
                 strlen(positions_str_arr[i]) > strlen(temp_string)
                     ? strlen(positions_str_arr[i]) - 1
                     : strlen(temp_string))) {
      duplicate_check = true;
      strcpy(*display_choice, "THIS POSITION ALREADY EXISTS!");
      display_screen(display_choice,
                     sizeof(display_choice) / sizeof(display_choice[0]) - 1);
      break;
    }
  }
  if (duplicate_check == false) {
    strcpy(positions_str_arr[*num_of_total_positions], temp_string);
    sprintf(*display_choice, "POSITION %s HAS BEEN ADDED",
            positions_str_arr[*num_of_total_positions]);
    display_screen(display_choice,
                   sizeof(display_choice) / sizeof(display_choice[0]) - 1);
    (*num_of_total_positions)++;
  }
  wait(3);
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
  int position_value = 0; /*Initialised to 0 to avoid bugs when user inputs
                             letters instead of numbers*/
  char throwaway_string[MAX_STRING_LENGTH];
  char display_choice[MAX_POSITIONS][MAX_STRING_LENGTH] = {"EXISTING POSITIONS",
                                                           " "};
  int i;

  for (i = 0; i < *num_of_total_positions; i++) {
    sprintf(display_choice[i + 2], "%d = %s", i + 1, positions_str_arr[i]);
    /* Remove newline from positions, so it will be printed correctly in the
     * display. We move the null character on top of the newline.
     * display_choice[i + 2] because we already initialized the array with two
     * values */
  }

  sprintf(display_choice[i + 2], " ");
  i++;
  sprintf(display_choice[i + 2],
          "ENTER DIGIT CORRESPONDING TO POSITION TO DELETE");

  display_screen(display_choice, i + 3); /* We added i plus 4(3) strings */
  scanf("%d", &position_value);
  fgets(throwaway_string, MAX_STRING_LENGTH, stdin);
  if (position_value >= 1 && position_value <= *num_of_total_positions) {
    sprintf(display_choice[0], "POSITION DELETED: %s",
            positions_str_arr[position_value - 1]);
    if (*num_of_total_positions == 1) {
      remove("positions.txt");
    } else {
      for (i = position_value - 1; i < *num_of_total_positions; i++)
        strcpy(positions_str_arr[i], positions_str_arr[i + 1]);
    }
    (*num_of_total_positions)--;
  } else
    sprintf(display_choice[0], "THE CHOSEN POSITION DOES NOT EXIST!");

  display_screen(display_choice, 0);
  wait(3);
}
