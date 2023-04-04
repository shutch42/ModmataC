#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

struct token_info {
	char** list;
	int count;
};

struct token_info parse(char* str, char delim);
int regex_cmp(char* str, char* expr);

#endif
