#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "stack.h"
#include "scanner.h"
#include "token.h"
#include "testScanner.h"
#include "parser.h"

void testScanner(char * input) {

	struct Token token;
	int left = 0, right = 0, line_number = 0;
	int * left_ptr = &left;
	int * right_ptr = &right;
	int * line_numberPtr = &line_number;
	struct node_t* root;


	//const char *token_Types[] = { "operator/delim_tK" , "identifier_tK" , "number_tK" , "EOF_tK", "keyword_tK" };



	// Get first token

	token = scanner(input, left_ptr, right_ptr, line_numberPtr);


	// Calls program, will recursively parse tokens
	root = program(&token, input, left_ptr, right_ptr, line_numberPtr);

	//printPreorder(root);

	printf("FINAL token: %s\n", token.instance);
	if (token.type == 3) {
		printf("Successful parse\n");
	}
	

	exit(0);
}



struct node_t* program(struct Token* token, char * str, int * left, int * right, int * line_number){

	printf("In program\n");
	printf("PROGRAM token.instance: %s\n", token->instance);
	char label[] = "label";

	struct node_t* node = get_Node(token, label);


	// Call vars on left child
	node->left_child = vars(token, str, left, right, line_number);


	// If 'main' token, then consume
	if (strcmp(token->instance, "main") == 0){
		(*token) = scanner(str, left, right, line_number);
	}


	// Call block on middle child
	node->middle_child = block(token, str, left, right, line_number);


	return node;
}



struct node_t* block(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In block\n");
	printf("BLOCK token.instance: %s\n", token->instance);
	char label[] = "block";
	struct node_t* node;


	// If 'begin' token then consume
	if (strcmp(token->instance, "begin") == 0){
		node = get_Node(token, label);
		(*token) = scanner(str, left, right, line_number);
	}
	else {
		printf("Error (block): Expecting begin keyword after main\n");
		exit(0);
	}


	// Call vars on left child
	node->left_child = vars(token, str, left, right, line_number);

	
	// Call stats on middle child
	node->middle_child = stats(token, str, left, right, line_number);


	// If 'end' token then consume
	if (strcmp(token->instance, "end") == 0){
		return node;
	}	


	return node;

}



struct node_t* vars(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In vars\n");
	printf("VARS token.instance: %s\n", token->instance);
	char label[] = "vars";
	struct node_t* node;	


	// If lookahead is not data, return
	if (strcmp(token->instance, "data") != 0){
		return NULL;
	}


	// 
	if(strcmp(token->instance, "data") == 0){
		node = get_Node(token, label);
		(*token) = scanner(str, left, right, line_number);
	}
	else{
		printf("Error (vars): Expecting data token\n");
		exit(0);
	}

	// If identifier store in node
	if (token->type == 1) {

		store_second_token(node, token);
		(*token) = scanner(str, left, right, line_number);

		if(strcmp(token->instance, "=") != 0) {
			printf("Error (vars): expecting =\n");
			exit(0);
		}
		
		// Consume = token
		(*token) = scanner(str, left, right, line_number);
		
	}
	else {
		printf("Error (vars): Expecting identifier after =\n");
		exit(0);
	}


	// If number store second token in same node
	if(token->type == 2) {
		printf("HERE\n");
		store_third_token(node, token);
		(*token) = scanner(str, left, right, line_number);
	}
	else{
		printf("Error (vars): expecting number token\n");
		exit(0);
	}

	// Check for ; at the end
	if(strcmp(token->instance, ";") == 0){
		(*token) = scanner(str, left, right, line_number);

	}
	else  {
		printf("Error (vars): expecting ; token after number\n");
		exit(0);
	}

	// Call vars with left child
	node->left_child = vars(token, str, left, right, line_number);

	
	
	return node;



}


struct node_t* expr(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In expr\n");
	printf("EXPR token.instance: %s\n", token->instance);
	char label[] = "expr";
	// Generate node, get lookahead
	struct node_t* node = gen_Node(label);
	struct Token lookahead = next_Token(str, left, right, line_number);


	// If lookahead is "-" then  call expr
	if (strcmp(lookahead.instance, "-") == 0){
		(*token) = scanner(str, left, right, line_number);
		(*token) = scanner(str, left, right, line_number);
		node->left_child = expr(token, str, left, right, line_number);
	}
	else{
		// If lookahead is something else, call N
		node->left_child = N(token, str, left, right, line_number);

	}

	return node;

}


struct node_t* N(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In N\n");
	printf("N token.instance: %s\n", token->instance);
	// Generate node, get lookahead
	char label[] = "N";
	struct node_t* node = gen_Node(label);
	struct Token lookahead = next_Token(str, left, right, line_number);

	
	// If next token is "/" call N() with left child.
	// If "*" call N() with left child
	// Else call A() with left child
	if (strcmp(lookahead.instance, "/") == 0){
		(*token) = scanner(str, left, right, line_number);
		(*token) = scanner(str, left, right, line_number);
		node = N(token, str, left, right, line_number);
	}
	else if (strcmp(lookahead.instance, "*") == 0){
		(*token) = scanner(str, left, right, line_number);
		(*token) = scanner(str, left, right, line_number);
		node->left_child = N(token, str, left, right, line_number);
	}
	else {
		node->left_child = A(token, str, left, right, line_number);

	}


	return node;

}


struct node_t* A(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In A\n");
	printf("A token.instance: %s\n", token->instance);
	// Generate node, get lookahead
	char label[] = "A";
	struct node_t* node = gen_Node(label);
	struct Token lookahead = next_Token(str, left, right, line_number);	
	

	// If next token "+" then call A(), else call M()
	if (strcmp(lookahead.instance, "+") == 0){
		(*token) = scanner(str, left, right, line_number);
		(*token) = scanner(str, left, right, line_number);
		node = A(token, str, left, right, line_number);
	}
	else if (strcmp(lookahead.instance, "-") == 0){
		(*token) = scanner(str, left, right, line_number);
		(*token) = scanner(str, left, right, line_number);
		node->left_child = A(token, str, left, right, line_number);
	}
	else {
		node->left_child = M(token, str, left, right, line_number);

	}

	
	return node;
}


struct node_t* M(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In M\n");
	printf("M token.instance: %s\n", token->instance);

	// Generate node
	char label[] = "M";
	struct node_t* node = gen_Node(label);
	
	

	// If current token * call M() with left_child, else call R() with left child
	if (strcmp(token->instance, "*") == 0){
		(*token) = scanner(str, left, right, line_number);
		node->left_child = M(token, str, left, right, line_number);
	}
	else {
		node->left_child = R(token, str, left, right, line_number);
	}


	return node;

}


struct node_t* R(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In R\n");
	printf("R token.instance: %s\n", token->instance);
	// Generate node
	char label[] = "R";
	struct node_t* node = gen_Node(label);



	// If open parenthesis, consume token and call expr, check for ) and return.
	if (strcmp(token->instance, "(") == 0){

		(*token) = scanner(str, left, right, line_number);
		node->left_child = expr(token, str, left, right, line_number);	


	}


	// If identifier, then store token in node
	if (token->type == 1){
		store_token(node, token);
		(*token) = scanner(str, left, right, line_number);


	}
	// If number, then store token in node
	else if (token->type == 2){
		store_token(node, token);
		(*token) = scanner(str, left, right, line_number);

	}


	if (strcmp(token->instance, ")") == 0){
		(*token) = scanner(str, left, right, line_number);
	}


	return node;


}


struct node_t* stats(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In stats\n");
	printf("STATS token.instance: %s\n", token->instance);	
	// Generate node, get lookahead
	char label[] = "stats";

	struct node_t* node = gen_Node(label);

	// Call stats with left child
	node->left_child = stat(token, str, left, right, line_number);
	
	// Call mStat with middle child
	node->middle_child = mStat(token, str, left, right, line_number);


	return node;

}


struct node_t* mStat(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In mStat\n");
	printf("MSTAT token.instance: %s\n", token->instance);	
	// Generate node and store token in it
	char label[] = "mStat";
	struct node_t* node = gen_Node(label);



	// Check if lookahead is end
	if (strcmp(token->instance, "end") == 0){
		store_token(node, token);
		(*token) = scanner(str, left, right, line_number);
		return node;
	}


	// Call stat with left child and mStat with middle
	node->left_child = stat(token, str, left, right, line_number);
	node->middle_child = mStat(token, str, left, right, line_number);

	return node;

}


struct node_t* stat(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In stat\n");
	printf("STAT token.instance: %s\n", token->instance);	
	char label[] = "stat";
	struct node_t* node = gen_Node(label);



	// Checks token for keywords, stores token and returns node.
	if(strcmp(token->instance, "getter") == 0){
		node->left_child = in(token, str, left, right, line_number);
		if (strcmp(token->instance, ";") != 0) {
			printf("Expecting ; at the end of getter statement\n");
			exit(0);
		}
		(*token) = scanner(str, left, right, line_number);
	}
	else if(strcmp(token->instance, "outter") == 0){
		node->left_child = out(token, str, left, right, line_number);
		if (strcmp(token->instance, ";") != 0) {
			printf("Expecting ; at the end of outter statement\n");
			exit(0);
		}
		(*token) = scanner(str, left, right, line_number);
	}
	else if(strcmp(token->instance, "begin") == 0){
		node->left_child = block(token, str, left, right, line_number);
	}
	else if(strcmp(token->instance, "if") == 0){
		node->left_child = if_(token, str, left, right, line_number);
		if (strcmp(token->instance, ";") != 0) {
			printf("Expecting ; at the end of if statement\n");
			exit(0);
		}
		(*token) = scanner(str, left, right, line_number);
	}
	else if(strcmp(token->instance, "loop") == 0){
		node->left_child = loop(token, str, left, right, line_number);
		if (strcmp(token->instance, ";") != 0) {
			printf("Expecting ; at the end of loop statement\n");
			exit(0);
		}
		(*token) = scanner(str, left, right, line_number);
	}
	else if(strcmp(token->instance, "assign") == 0){
		node->left_child = assign(token, str, left, right, line_number);
		if (strcmp(token->instance, ";") != 0) {
			printf("Expecting ; at the end of assign statement\n");
			exit(0);
		}
		(*token) = scanner(str, left, right, line_number);
	}
	else if(strcmp(token->instance, "void") == 0){
		node->left_child = label_(token, str, left, right, line_number);
		if (strcmp(token->instance, ";") != 0) {
			printf("Expecting ; at the end of void statement\n");
			exit(0);
		}
		(*token) = scanner(str, left, right, line_number);		
	}
	else if(strcmp(token->instance, "proc") == 0){
		node->left_child = goto_(token, str, left, right, line_number);
		if (strcmp(token->instance, ";") != 0) {
			printf("Expecting ; at the end of proc statement\n");
			exit(0);
		}
		(*token) = scanner(str, left, right, line_number);
	}
	else if(strcmp(token->instance, "main") == 0){
		(*token) = scanner(str, left, right, line_number);
	}
	else if(strcmp(token->instance, "begin") == 0){
		(*token) = scanner(str, left, right, line_number);
	}
	else if(strcmp(token->instance, "end") == 0){
		//(*token) = scanner(str, left, right, line_number);
	}
	else {
		printf("STAT token.instance: %s\n", token->instance);
		printf("Error (stat): Expecting keyword\n");
		exit(0);
	}

	return node;


}


struct node_t* in(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In in\n");
	printf("IN token.instance: %s\n", token->instance);

	// Generate node and store in token
	char label[] = "in";
	struct node_t* node = get_Node(token, label);

	// Consume in token
	(*token) = scanner(str, left, right, line_number);	


	// Check for identifier
	if (token->type == 1) {

		// Store then consume identifier token		
		store_second_token(node, token);
		(*token) = scanner(str, left, right, line_number);

	}
	else if (token->type != 1) {
		printf("Error (in): Expecting Identifier after getter");
		exit(0);
	}

	return node;

}


struct node_t* out(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In out\n");
	printf("OUT token.instance: %s\n", token->instance);

	// Generate node, store out label and outter token
	char label[] = "out";
	struct node_t* node = get_Node(token, label);

	// Consume outter token
	(*token) = scanner(str, left, right, line_number);


	// Call expr on left child
	node->left_child = expr(token, str, left, right, line_number);


	return node;
}


struct node_t* if_(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In if_\n");
	printf("IF token.instance: %s\n", token->instance);

	// Generate node, store if  label, and store if token
	char label[] = "if";
	struct node_t* node = get_Node(token, label);


	// Consume if
	(*token) = scanner(str, left, right, line_number);


	// If next token is [ call expr, RO, and expr on children and consume token
	if (strcmp(token->instance, "[") == 0){
		node->left_child = expr(token, str, left, right, line_number);
		node->middle_child = RO(token, str, left, right, line_number);
		node->right_child = expr(token, str, left, right, line_number);
	}
	else {
		printf("Error (if_): Expecting open bracket after if keyword.\n");
		exit(0);
	}


	// If ] consume and get new lookahead.
	if (strcmp(token->instance, "]") == 0){
		(*token) = scanner(str, left, right, line_number);
	}
	else {
		printf("Error (if_): Expecting closed bracket.\n");
		exit(0);
	}

	// If next token is "then", consume and call stat with far right child
	if (strcmp(token->instance, "then") == 0){
		store_second_token(node, token);
		(*token) = scanner(str, left, right, line_number);
		node->far_right_child = stat(token, str, left, right, line_number);
			
	}



	return node;


}


struct node_t* loop(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In loop\n");
	printf("LOOP token.instance: %s\n", token->instance);

	// Generate node, store if  label, and store if token
	char label[] = "loop";
	struct node_t* node = get_Node(token, label);

	// Consume loop token and get lookahead
	(*token) = scanner(str, left, right, line_number);

	// Check for [ and consume
	if (strcmp(token->instance, "[") == 0) {

		// Store loop token and call expr, RO, and expr with children
		(*token) = scanner(str, left, right, line_number);
		
		node->left_child = expr(token, str, left, right, line_number);
		node->middle_child = RO(token, str, left, right, line_number);
		node->right_child = expr(token, str, left, right, line_number);




	}
	else {
		printf("Error (loop): Expecting open bracket token.\n");
		exit(0);
	}


	// If token is ] then consume
	if (strcmp(token->instance, "]") == 0){
		(*token) = scanner(str, left, right, line_number);			
	}
	else {
		printf("Error (loop): Expecting closed bracket after open bracket.\n");
		exit(0);
	}



	node->far_right_child = stat(token, str, left, right, line_number);


	return node;


}


struct node_t* assign(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In assign\n");
	printf("ASSIGN token.instance: %s\n", token->instance);

	// Generate node, store assign label, store assign token, then consume
	char label[] = "assign";
	struct node_t* node = get_Node(token, label);
	(*token) = scanner(str, left, right, line_number);


	// If identifier, store in node, and consume token
	if (token->type == 1) {
		store_second_token(node, token);
		(*token) = scanner(str, left, right, line_number);	
	}
	else {
		printf("Erros (assign): Expecting identifier token after assign keywork\n");
		exit(0);
	}

	// If number store in node
	if(strcmp(token->instance, "=") == 0) {
		(*token) = scanner(str, left, right, line_number);	
		node->left_child = expr(token, str, left, right, line_number);
	}
	else{
		printf("Error (assign): expecting '=' after identifier token\n");
		exit(0);
	}


	return node;


}


struct node_t* RO(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In RO\n");
	printf("RO token.instance: %s\n", token->instance);	

	// Generate node, store assign label, and store assign token
	char label[] = "RO";
	struct node_t* node = get_Node(token, label);


	if(strcmp(token->instance, "=>") == 0){
		(*token) = scanner(str, left, right, line_number);
		return node;
	}
	else if(strcmp(token->instance, "=<") == 0){
		(*token) = scanner(str, left, right, line_number);
		return node;
	}
	else if(strcmp(token->instance, "==") == 0){
		(*token) = scanner(str, left, right, line_number);
		return node;
	}
	else if(strcmp(token->instance, "[") == 0){
	(*token) = scanner(str, left, right, line_number);
		if(strcmp(token->instance, "==") == 0){
			store_second_token(node, token);
			(*token) = scanner(str, left, right, line_number);
		}
		else {
			printf("Error (RO): expecting '==' after open bracket\n");
			exit(0);
		}

		if(strcmp(token->instance, "]") == 0){

		}
		else {
			printf("Error (RO): expecting closed bracket after ==\n");
			exit(0);
		}
	}
	else if(strcmp(token->instance, "%") == 0){
		(*token) = scanner(str, left, right, line_number);
		return node;
	}
	else if(strcmp(token->instance, "=") == 0){
		(*token) = scanner(str, left, right, line_number);
		return node;
	}

	return node;
}


struct node_t* label_(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In label_\n");
	printf("LABEL token.instance: %s\n", token->instance);	

	// Generate node, store label label, store void token, then consume
	char label[] = "label";
	struct node_t* node = get_Node(token, label);
	(*token) = scanner(str, left, right, line_number);



	// If identifier store in node
	if (token->type == 1) {
		store_second_token(node, token);
		(*token) = scanner(str, left, right, line_number);
	}
	else {
		printf("Error (label_): Expecting identifier token after void keyword\n");
		exit(0);
	}

	return node;
}


struct node_t* goto_(struct Token* token, char * str, int * left, int * right, int * line_number){
	printf("In goto_\n");	
	printf("GOTO token.instance: %s\n", token->instance);

	// Generate node, store goto label, store proc token, then consume
	char label[] = "goto";
	struct node_t* node = get_Node(token, label);
	(*token) = scanner(str, left, right, line_number);
	


	// If identifier store in node
	if (token->type == 1) {
		store_second_token(node, token);	
		(*token) = scanner(str, left, right, line_number);
	}
	else {
		printf("Error (goto_): Expecting identifier after proc keyword\n");
		exit(0);
	}

	return node;
}


// This function stores a token and sets children  to null.
// Use store_token() if you want to only store a token.
struct node_t* get_Node(struct Token* token, char * label){
	
	// Allocate space for note_t object
	struct node_t* node = (struct node_t*)malloc(sizeof(struct node_t));

	// Allocate space for token instance string and copy from token to node's token
	node->token.instance = malloc(strlen(token->instance) * sizeof (char));;
	strcpy(node->token.instance, token->instance);

	// Copy token type and line number
	node->token.type = token->type;
	node->token.line_number = token->line_number;

	// Set children to null
	node->left_child = NULL;
	node->middle_child = NULL;
	node->right_child = NULL;
	node->far_right_child = NULL;

	// Set flags, since we stored one token, tkflg1 = 1
	node->tkflg1 = 1;
	node->tkflg2 = 0;
	node->tkflg3 = 0;


	// Allocate space for token instance string and copy from token to node's token
	node->label = malloc(strlen(label) * sizeof (char));
	strcpy(node->label, label);


	return node;


}


// Simply allocates space for node and sets children to null
struct node_t* gen_Node(char * label){
	
	// Allocate space for note_t's token and copy token to it
	struct node_t* node = (struct node_t*)malloc(sizeof(struct node_t));

	// Set children to null
	node->left_child = NULL;
	node->middle_child = NULL;
	node->right_child = NULL;
	node->far_right_child = NULL;


	// Allocate space for label string and copy
	node->label = malloc(strlen(label) * sizeof (char));
	strcpy(node->label, label);

	// Set flags to zero
	node->tkflg1 = 0;
	node->tkflg2 = 0;
	node->tkflg3 = 0;

	return node;


}

// Just stores second token in node
void store_second_token(struct node_t* node, struct Token* token){
	
	// Allocate space for note_t's token and copy token to it
	//node = (struct node_t*)malloc(sizeof(struct node_t));

	// Allocate space for token instance string and copy from token to node's token
	node->second_token.instance = malloc(strlen(token->instance) * sizeof (char));
	strcpy(node->second_token.instance, token->instance);

	// Copy token type and line number
	node->second_token.type = token->type;
	node->second_token.line_number = token->line_number;

	node->tkflg2 = 1;


}

// Just stores third token in node
void store_third_token(struct node_t* node, struct Token* token){
	
	// Allocate space for note_t's token and copy token to it
	//node = (struct node_t*)malloc(sizeof(struct node_t));

	// Allocate space for token instance string and copy from token to node's token
	node->third_token.instance = malloc(strlen(token->instance) * sizeof (char));
	strcpy(node->third_token.instance, token->instance);

	// Copy token type and line number
	node->third_token.type = token->type;
	node->third_token.line_number = token->line_number;

	node->tkflg3 = 1;


}

// Stores token in node
void store_token(struct node_t* node, struct Token* token){
	
	// Allocate space for note_t's token and copy token to it
	//node = (struct node_t*)malloc(sizeof(struct node_t));

	// Allocate space for token instance string and copy from token to node's token
	node->token.instance = malloc(strlen(token->instance) * sizeof (char));
	strcpy(node->token.instance, token->instance);

	// Copy token type and line number
	node->token.type = token->type;
	node->token.line_number = token->line_number;

	node->tkflg1 = 1;


}

// Lookahead function, calls scanner and resets to previous values, as if it never got next token.
struct Token next_Token(char * str, int * left, int * right, int * line_number){

	int temp_left; 
	int temp_right;
	int temp_line_number;

	temp_left = (*left);
	temp_right = (*right);
	temp_line_number = (*line_number);



	//struct Token* next = (struct Token*)malloc(sizeof(struct Token));
	struct Token next;
	next = scanner(str, left, right, line_number);

	//next->instance = malloc(strlen(token->instance) * sizeof (char));
	//strcpy(next->instance, token->instance);


	//next->type = token->type;
	//next->line_number = token->line_number;

	(*left) = temp_left;
	(*right) = temp_right;
	(*line_number) = temp_line_number;

	return next;


}


void printPreorder(struct node_t* node){
	
	if (node != NULL) {
		printf("%s\n", node->token.instance);

		printPreorder(node->left_child);

		printPreorder(node->middle_child);

		printPreorder(node->right_child);

		printPreorder(node->far_right_child);
	
	}

}

