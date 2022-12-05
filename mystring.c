/**
 * -----------------------------------------------------------------
 * IFJ22 compiler implementation
 * @file mystring.c
 * @authors Stefan Peknik xpekni01
 * @brief mystring
 * @date 2022-11-16
 * @copyright Copyright (c) 2022
 * -----------------------------------------------------------------
 */
#include "mystring.h"
#include "error.h"
#include "logger.h"

/**
 * @brief Function to create new string
 * 
 * @return string* 
 */
string *SetupString() {
  string *str = (string *)malloc(sizeof(struct string));
  if (str == NULL) ErrorExit(99, "Malloc failed!");
  str->data = (char *)malloc(sizeof(char));
  if (str->data == NULL) ErrorExit(99, "Malloc failed!");
  str->data[0] = '\0';
  str->size = 1;
  logger("SetupString", "String initialized");
  return str;
}
/**
 * @brief Function to add char to string
 * 
 * @param str 
 * @param ch 
 * @return string* 
 */
string *AddToString(string *str, char ch) {
  str->size++;
  char *tmp = realloc(str->data, str->size * sizeof(char));
  if (!tmp) {
    free(str->data);
    str->data = NULL;
    ErrorExit(99, "Malloc failed!");
  }
  str->data = tmp;
  str->data[str->size - 2] = ch;
  logger("AddToString", "Character added");
  return str;
}
/**
 * @brief Replace char in string
 * 
 * @param str 
 * @param index 
 * @param ch 
 * @return string* 
 */
string *ReplaceCharInString(string *str, size_t index, char ch) {
  if (index > str->size) return str;
  if (ch == '\0') {
    str->size = index + 1;
    char *tmp = realloc(str->data, str->size * sizeof(char));
    if (!tmp) {
      free(str->data);
      str->data = NULL;
      ErrorExit(99, "Malloc failed!");
    }
    str->data = tmp;
  }
  str->data[index] = ch;
  logger("ReplaceCharInString", "Character replaced");
  return str;
}
/**
 * @brief Resets string
 * 
 * @param str 
 * @return string* 
 */
string *ResetString(string *str) {
  free(str->data);
  logger("ResetString", "String freed");
  return SetupString();
}
/**
 * @brief Concatenates two strings
 * 
 * @param str 
 * @param used 
 * @return string* 
 */
string *ConcatString(string *str, char *used) {
  for (size_t i = 0; i < strlen(used); i++) {
    str = AddToString(str, used[i]);
  }
  return str;
}