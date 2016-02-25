typedef struct token
{
	int tokenType; // 0 - number, 1 - character
	char symbol;
	int value;
} token_t;