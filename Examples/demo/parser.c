#include "parser.h"

struct token_info parse(char* str, char delim) {
	struct token_info tokens;

	// Copy string argument into a new string so that we don't modify the original on accident
	char* str_copy = malloc(sizeof(char)*80);
	memset(str_copy, '\0', sizeof(str_copy));
	strcpy(str_copy, str);
	
	char* token = strtok(str_copy, &delim);
	tokens.list = malloc(sizeof(char*) * 50); // Max of 50 tokens
	tokens.count = 0;

	while(token != NULL) {
		tokens.list[tokens.count] = malloc(sizeof(char) * 80); // Max of 80 characters per word
		strcpy(tokens.list[tokens.count], token);
		tokens.count++;
		token = strtok(NULL, &delim);
	}
	free(str_copy);
	return tokens;
}

int regex_cmp(char* str, char* expr) {
	// Compile regex exression
	regex_t r;
	if(regcomp(&r, expr, REG_EXTENDED)) {
		printf("Regex did not compile: %s\n", expr);
		return 0;
	}
	
	// Compare regex expression w/ string
	int result = regexec(&r, str, 0, NULL, 0);
	if(result == 0) {
	//	printf("Regex matches up: %s, %s\n", str, expr);
		return 1;
	}
	else if (result == REG_NOMATCH) {
	//	printf("Regex does not match: %s, %s\n", str, expr);
		return 0;
	}
	else {
		printf("Error occurred in regex comparison\n");
		return 0;
	}
}
