#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>

#include "token.h"
#include "testScanner.h"
#include "stack.h"

bool isDelmiter(char ch);
bool isOperator(char ch);
bool validIdentifier(char * str);
bool isKeyword(char * str);
bool isInteger(char * str);
bool isRealNumber(char * str);
char * subString(char * str, int left, int right);
struct Token scanner(char * str, int * left, int * right, int * line_number);


#endif
