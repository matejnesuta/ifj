#ifndef __MYSTRING_H__
#define __MYSTRING_H__

#include "include.h"

/**
 * @struct string
 * 
 */
typedef struct string {
  char *data;
  size_t size;
} string;

string *SetupString();
string *AddToString(string *, char);
string *ReplaceCharInString(string *, size_t, char);
string *ResetString(string *);
string *ConcatString(string *, char *);


#endif