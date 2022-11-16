#ifndef __MYSTRING_H__
#define __MYSTRING_H__

#include "include.h"

// string stuff
typedef struct string {
  char *data;
  size_t size;
} string;

string SetupString();
string AddToString(string str, char ch);
string ReplaceCharInString(string str, size_t index, char ch);
string ResetString(string str);
// string stuff

#endif