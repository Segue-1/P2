#ifndef TOKEN_H
#define TOKEN_H

struct Token {

	int type;
	char * instance;
	int line_number;

};


enum TokenType {

	OPERATOR_DELIM_TK,
	IDENTIFIER_TK,
	NUMBER_TK,
	END_OF_FILE_TK,
	KEYWORD_TK

};

#endif
