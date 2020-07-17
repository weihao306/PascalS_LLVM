#ifndef HANDLE_H
#define HANDLE_H

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <stdarg.h>
#include <float.h>
#include "ast.h"
#include "parser.hpp"

extern FILE* file;
extern int ParseError;
extern bool isdebug;

extern void Init_handle(void);
extern void Close_handle(void);
extern void PrintRow(void); 
extern int GetNextChar(char *b, int maxBuffer);
extern void BeginToken(char*);
extern void PrintError(char const *s, ...);

#endif