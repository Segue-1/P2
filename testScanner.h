#ifndef TESTSCANNER_H
#define TESTSCANNER_H

#include "token.h"
#include "stack.h"
#include "testScanner.h"
#include "scanner.h"

struct node_t {

	// Tokens for storage
	struct Token token;
	struct Token second_token;
	struct Token third_token;

	// Label for nonterminals
	char * label;

	// Way too many children...
	struct node_t *left_child;
	struct node_t *middle_child;
	struct node_t *right_child;
	struct node_t *far_right_child;

	// Tree level and flags for tokens
	int tree_level;
	int tkflg1;
	int tkflg2;
	int tkflg3;

};




void testScanner(char * input);
struct node_t* program(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* block(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* vars(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* expr(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* N(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* A(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* M(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* R(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* stats(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* mStat(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* stat(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* in(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* out(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* if_(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* loop(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* assign(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* RO(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* label_(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* goto_(struct Token* token, char * str, int * left, int * right, int * line_number);
struct node_t* get_Node(struct Token* token, char * label);
struct node_t* gen_Node(char * label);
void store_second_token(struct node_t* node, struct Token* token);
void store_third_token(struct node_t* node, struct Token* token);
void store_token(struct node_t* node, struct Token* token);
struct Token next_Token(char * str, int * left, int * right, int * line_number);
void calc_depth(struct node_t* node, int depth);
void printPreorder(struct node_t* node);

#endif
