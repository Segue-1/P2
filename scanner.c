#include <stdbool.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 

#include "scanner.h"
#include "token.h"


// Returns 'true' if the character is a DELIMITER. 
bool isDelimiter(char ch) 
{ 
	if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' || 
		ch == '/' || ch == ',' || ch == ';' || ch == '>' || 
		ch == '<' || ch == '=' || ch == '(' || ch == ')' || 
		ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
		ch == '.') 
		return (true); 
	return (false); 
} 


// Returns 'true' if the character is an OPERATOR. 
bool isOperator(char ch) 
{ 
	if (ch == '+' || ch == '-' || ch == '*' || 
	    ch == '/' || ch == '>' || ch == '<' || 
	    ch == '=' || ch == '%') 
		return (true); 
	return (false); 
} 


// Returns 'true' if the string is a VALID IDENTIFIER. 
bool validIdentifier(char* str) 
{ 


	if (str[0] == '0' || str[0] == '1' || str[0] == '2' || 
		str[0] == '3' || str[0] == '4' || str[0] == '5' || 
		str[0] == '6' || str[0] == '7' || str[0] == '8' || 
		str[0] == '9' || isDelimiter(str[0]) == true) 
		return (false); 
	return (true); 
} 


// Returns 'true' if the string is a KEYWORD. 
bool isKeyword(char* str) 
{ 
	if (!strcmp(str, "if") || !strcmp(str, "begin") || !strcmp(str, "end")
		|| !strcmp(str, "loop") || !strcmp(str, "whole")
		|| !strcmp(str, "void") || !strcmp(str, "exit")
		|| !strcmp(str, "getter") || !strcmp(str, "outter")
		|| !strcmp(str, "main") || !strcmp(str, "then")
		|| !strcmp(str, "assign") || !strcmp(str, "data")
		|| !strcmp(str, "proc")) 
		return (true); 
	return (false); 
}

bool isInteger(char * str) 
{

	int i, len = strlen(str);
	if (len == 0){
		return(false);
	}
	for(i = 0; i < len; i++) {
		if ((str[i] != '0' && str[i] != '1' && str[i] != '2'
			&& str[i] != '3' && str[i] != '4' && str[i] != '5'
			&& str[i] != '6' && str[i] != '7' && str[i] != '8'
			&& str[i] != '9') || (str[i] == '-' && i > 0)){
			return(false);
			}
	}
	return(true);

} 

bool isRealNumber(char * str) 
{

	int i, len = strlen(str);
	bool hasDecimal = false;

	if(len == 0)
		return(false);
	for(i = 0; i < len; i++) {
		if ((str[i] != '0' && str[i] != '1' && str[i] != '2' 
			&& str[i] != '3' && str[i] != '4' && str[i] != '5'
			&& str[i] != '6' && str[i] != '7' && str[i] != '8'
			&& str[i] != '9' && str[i] != '.') || (str[i] == '-' && i > 0))
			return(false);
		if(str[i] == '.')
			hasDecimal = true;


	}
	return(hasDecimal);

}

bool isEOF(char * str) {

	int i, len = strlen(str);

	if (len != 3)
		return(false);
	for(i = 0; i < len; i++) {
		if (str[i] == 'E' && str[i + 1] == 'O' && str[i + 2]  == 'F')
			return(true);	

	}

	return(false);

}

char * subString(char * str, int left, int right)
{

	int i;
	char * subStr = (char*)malloc(sizeof(char) * (right - left +2));

	for(i = left; i <= right; i++)
		subStr[i - left] = str[i];
	subStr[right - left + 1] = '\0';
	return(subStr);

}


// Scan the input string. 
struct Token scanner(char* str, int * left, int * right, int * line_number) 
{ 

	int len = strlen(str); 
	struct Token token;

	
    	// While right is <= length and left <= right
	while ((*right) <= len && (*left) <= (*right)) {
		if (str[(*right)] == '\n'){
			(*line_number)++;
			(*right)++;
			(*left) = (*right);
		}
		// If $$ detected skip over it until $$ is found again or we reach end of string
		if (str[(*right)] == '$' && str[(*right + 1)] == '$') {
			
			do
			{
			(*right)++;
			}while (str[(*right)] != '$' && str[(*right) + 1] != '$' && (*right) < len);
			
			(*right)++; 
			(*left) = (*right);
	
			
		} 	
	    	// Check if str[right] is DELIMITER
		if (isDelimiter(str[(*right)]) == false){
		    	// If it isn't, increment right
			(*right)++; 
		}
        	// if str[right] is a delimiter and left = right
		if ((isDelimiter(str[(*right)]) == true) && ((*left) == (*right))) {
		    	// if str[right] is operator
			if (isOperator(str[(*right)]) == true && isOperator(str[(*right) + 1]) == true){

				token.type = 0;
				token.instance = malloc(3 * sizeof (char));
				token.instance[0] = str[(*right)];
				token.instance[1] = str[(*right) + 1];
				token.instance[2] = '\0';
				token.line_number = (*line_number);					
				(*right)++;
				(*right)++;
				(*left) = (*right);
				break;
			}			
			if (isOperator(str[(*right)]) == true){
			
				token.type = 0;
				token.instance = malloc(2 * sizeof (char));
				token.instance[0] = str[(*right)];
				token.instance[1] = '\0';
				token.line_number = (*line_number);				
				(*right)++;
				(*left) = (*right);
				break;
			}
			if(isOperator(str[(*right)]) == false && isDelimiter(str[(*right)]) == true && str[(*right)] != ' '){
	
				token.type = 0;
				token.instance = malloc(2 * sizeof (char));
				token.instance[0] = str[(*right)];
				token.instance[1] = '\0';
				token.line_number = (*line_number);				
				(*right)++;
				(*left) = (*right);			
				break;
			}
            		// increment right
			(*right)++; 
			// set left = right
			(*left) = (*right); 
		}

		// else if str[right] is a delimeter and left != right OR 
		// right = length and left != right
		else if (((isDelimiter(str[(*right)]) == true) && ((*left) != (*right))) 
				|| ((*right) == len && (*left) != (*right))) {
 
			// Get subString
			char* subStr = subString(str, (*left), (*right) - 1); 
        
            		// Check what subString is
			if (isKeyword(subStr) == true){ 

				token.type = 4;				
				token.instance = malloc(strlen(subStr) * sizeof (char));
				strcpy(token.instance, subStr);
				token.line_number = (*line_number);
				(*left) = (*right);
				break;
			}
			else if (isInteger(subStr) == true) {

				token.type = 2;	
				token.instance = malloc(strlen(subStr) * sizeof (char));
				strcpy(token.instance, subStr);
				token.line_number = (*line_number);
				(*left) = (*right);
				break;
			}
			else if (isRealNumber(subStr) == true) {

				token.type = 2;				
				token.instance = malloc(strlen(subStr) * sizeof (char));
				strcpy(token.instance, subStr);
				token.line_number = (*line_number);		

				(*left) = (*right);
				break;
			}
			else if (validIdentifier(subStr) == true
				 && isDelimiter(str[(*right) - 1]) == false) {
	
				if (subStr[0] == 'E' && subStr[1] == 'O' && subStr[2] == 'F'){
					token.type = 3;
					token.instance = malloc(strlen(subStr) * sizeof (char));
					strcpy(token.instance, subStr);
					token.line_number = (*line_number);
					(*left) = (*right);
					break;
				}
				if (subStr[0] == '\n') {
					(*left) = (*right);
					break;
				}	

				token.type = 1;				
				token.instance = malloc(strlen(subStr) * sizeof (char));
				strcpy(token.instance, subStr);
				token.line_number = (*line_number);
				(*left) = (*right);
				break;	
			}
			else if (validIdentifier(subStr) == false
					&& isDelimiter(str[(*right) - 1]) == false) {

				if (subStr[0] == 'E' && subStr[1] == 'O' && subStr[2] == 'F'){
					token.type = 3;
					token.instance = malloc(strlen(subStr) * sizeof (char));
					strcpy(token.instance, subStr);
					token.line_number = (*line_number);
					(*left) = (*right);
					break;
				}				


				token.type = -1;				
				token.instance = malloc(strlen(subStr) * sizeof (char));
				strcpy(token.instance, subStr);
				token.line_number = (*line_number);
				(*left) = (*right);
				break;			
			} 
			// Set left = right
			// Do this until right is > length or left > right
			(*left) = (*right); 
		}

	} 
	return token; 
}
