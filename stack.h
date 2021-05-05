#ifndef STACK_H
#define STACK_H

#include "token.h"

struct token_Stack {

	int top;
	unsigned capacity;
	struct Token* array;

};

struct token_Stack* createStack(unsigned capacity);
int isFull(struct token_Stack* token_Stack);
int isEmpty(struct token_Stack* token_Stack);
void push(struct token_Stack* token_Stack, struct Token token);
void pop(struct token_Stack* token_Stack);
struct Token* peek(struct token_Stack* token_Stack);

#endif

