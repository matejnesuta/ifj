#include "logger.h"

#include <stdio.h>
#include <stdlib.h>

void logger(const char* tag, const char* message) {
  return;
  printf("[%s]: %s\n", tag, message);
}