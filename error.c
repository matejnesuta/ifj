/**
 * -----------------------------------------------------------------
 * IFJ22 compiler implementation
 * @file error.c
 * @authors Stefan Peknik xpekni01
 * @brief error
 * @date 2022-12-01
 * @copyright Copyright (c) 2022
 * -----------------------------------------------------------------
 */
#include "error.h"

#include "include.h"


/**
 * @brief Function to exit with error
 *
 * @param exit_code
 * @param message
 */
void ErrorExit(int exit_code, char *message) {
  fprintf(stderr, "%s\n", message);
  fprintf(stderr, "EXIT %d\n", exit_code);
  exit(exit_code);
}
