#include "mystring.h"

#include "logger.h"

string *SetupString() {
  string *str = (string *)malloc(sizeof(struct string));
  if (str == NULL) exit(99);
  str->data = (char *)malloc(sizeof(char));
  if (str->data == NULL) exit(99);
  str->data[0] = '\0';
  str->size = 1;
  logger("SetupString", "String initialized");
  return str;
}

string *AddToString(string *str, char ch) {
  str->size++;
  char *tmp = realloc(str->data, str->size * sizeof(char));
  if (!tmp) {
    free(str->data);
    str->data = NULL;
    exit(99);
  }
  str->data = tmp;
  str->data[str->size - 2] = ch;
  logger("AddToString", "Character added");
  return str;
}

string *ReplaceCharInString(string *str, size_t index, char ch) {
  if (index > str->size) return str;
  if (ch == '\0') {
    str->size = index + 1;
    char *tmp = realloc(str->data, str->size * sizeof(char));
    if (!tmp) {
      free(str->data);
      str->data = NULL;
      exit(99);
    }
    str->data = tmp;
  }
  str->data[index] = ch;
  logger("ReplaceCharInString", "Character replaced");
  return str;
}

string *ResetString(string *str) {
  free(str->data);
  logger("ResetString", "String freed");
  return SetupString();
}

string *ConcatString(string *str, char *used) {
  for (size_t i = 0; i < strlen(used); i++) {
    str = AddToString(str, used[i]);
  }
  return str;
}