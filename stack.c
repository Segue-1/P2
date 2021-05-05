#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "scanner.h"
#include "testScanner.h"


// adapted from https://www.geeksforgeeks.org/stack-data-structure-introduction-program/

 
// function to create a stack of given capacity. It initializes size of
// stack as 0
struct token_Stack* createStack(unsigned capacity)
{
    struct token_Stack* token_Stack = (struct token_Stack*)malloc(sizeof(struct token_Stack));
    token_Stack->capacity = capacity;
    token_Stack->top = -1;
    token_Stack->array = (struct Token*)malloc(token_Stack->capacity * sizeof(struct Token));
    return token_Stack;
}
 
// Stack is full when top is equal to the last index
int isFull(struct token_Stack* token_Stack)
{
    return token_Stack->top == token_Stack->capacity - 1;
}
 
// Stack is empty when top is equal to -1
int isEmpty(struct token_Stack* token_Stack)
{
    return token_Stack->top == -1;
}
 
// Function to add an item to stack.  It increases top by 1
void push(struct token_Stack* token_Stack, struct Token token)
{
	
	const char *token_Types[] = { "operator/delim_tK" , "identifier_tK" , "number_tK" , "EOF_tK", "keyword_tK"};


	if (isFull(token_Stack))
		return;

        token_Stack->array[++token_Stack->top].type = token.type;
	token_Stack->array[token_Stack->top].instance = malloc(strlen(token.instance) * sizeof (char));
	strcpy(token_Stack->array[token_Stack->top].instance, token.instance);
	token_Stack->array[token_Stack->top].line_number = token.line_number;
    	printf("  Token instance: %s\n  Token type: %s\n  Has been pushed to stack\n\n", token.instance, token_Types[token.type]);

}
 
// Function to remove an item from stack.  It decreases top by 1
void pop(struct token_Stack* token_Stack)
{
    if (isEmpty(token_Stack))
        return;
    token_Stack->top--;
}
 
// Function to return the top from stack without removing it
struct Token* peek(struct token_Stack* token_Stack)
{

    if (isEmpty(token_Stack))
        return NULL;

	struct Token *token_Ptr;
	token_Ptr = &token_Stack->array[token_Stack->top];


    return token_Ptr;
}
