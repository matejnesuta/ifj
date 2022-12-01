#include "error.h"

#include "include.h"

void ErrorExit(int exit_code, char *message) {
  fprintf(stderr, "%s\n", message);
  fprintf(stderr, "EXIT %d\n", exit_code);
  exit(exit_code);
}