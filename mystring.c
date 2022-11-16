#include "mystring.h"

string SetupString() {
  string str;
  str.data = (char *)malloc(sizeof(char));
  if (str.data == NULL) exit(99);
  str.data[0] = '\0';
  str.size = 1;
  return str;
}

string AddToString(string str, char ch) {
  str.size++;
  char *tmp = realloc(str.data, str.size * sizeof(char));
  if (!tmp) {
    free(str.data);
    str.data = NULL;
    exit(99);
  }
  str.data = tmp;
  str.data[str.size - 2] = ch;
  return str;
}

string ReplaceCharInString(string str, size_t index, char ch) {
  if (index > str.size) return str;
  if (ch == '\0') {
    str.size = index + 1;
    char *tmp = realloc(str.data, str.size * sizeof(char));
    if (!tmp) {
      free(str.data);
      str.data = NULL;
      exit(99);
    }
    str.data = tmp;
  }
  str.data[index] = ch;
  return str;
}

string ResetString(string str) {
  free(str.data);
  return SetupString();
}