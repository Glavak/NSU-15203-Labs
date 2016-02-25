#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include "token.h"

signed int getNumber(char c)
{
	if (c >= '0' && c <= '9') return (signed int)(c - '0');
	else return -1;
}

signed int getOperatorPriority(char op)
{
	switch (op)
	{
	case '^':
		return 84;
	case '*':
	case '/':
		return 42;
	case '+':
	case '-':
		return 21;
	case '(':
	case ')':
		return 10;
	default:
		return -1;
	}
}

// returns 1, if left-to-right, -1 if right-to-left
signed int getOperatorAssociativity(char op)
{
	switch (op)
	{
		case '^':
			return -1;
		case '*':
		case '/':
		case '+':
		case '-':
			return 1;
		default:
			return 0;
	}
}

double power(double a, double b)
{
	double result = 1;
	if (b > 0)
	{
		for (int i = 0; i < b; i++) result *= a;
	}
	else
	{
		for (int i = 0; i < -b; i++) result /= a;
	}
	return result;
}

double calculateOperator(char op, double a, double b)
{
	switch (op)
	{
	case '*':
		return a * b;
	case '/':
		return a / b;
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '^':
		return power(a, b);
	default:
		// We should neveer get here
		assert(0);
		return -1;
	}
}

token_t *parse(char* string, int *tokensCount)
{
	token_t *result = malloc(sizeof(token_t) * strlen(string));
	*tokensCount = 0;

	for (size_t i = 0; i < strlen(string); i++)
	{
		if (getNumber(string[i]) == -1)
		{
			if (getOperatorPriority(string[i]) == -1) return NULL; // Invalid operator

			result[*tokensCount].tokenType = 1;
			result[*tokensCount].symbol = string[i];
		}
		else
		{
			int boof = getNumber(string[i]);
			while (i + 1 < strlen(string) && getNumber(string[i + 1]) != -1)
			{
				i++;
				boof *= 10;
				boof += getNumber(string[i]);
			}
			result[*tokensCount].tokenType = 0;
			result[*tokensCount].value = boof;
		}
		(*tokensCount)++;
	}

	return result;
}

token_t *convertToRPN(token_t *expression, int expressionLength, int *resultLength)
{
	token_t *result = malloc(sizeof(token_t) * expressionLength);
	*resultLength = 0;
	token_t *stack = malloc(sizeof(token_t) * expressionLength);
	int stackEnd = -1;

	for (int i = 0; i < expressionLength; i++)
	{
		if (expression[i].tokenType == 0)
		{
			result[(*resultLength)++] = expression[i];
		}
		else if (expression[i].tokenType == 1 && expression[i].symbol == '(')
		{
			if (i < expressionLength-1 && expression[i + 1].tokenType == 1 && expression[i + 1].symbol == ')') return NULL; // "()"
			stack[++stackEnd] = expression[i];
		}
		else if (expression[i].tokenType == 1 && expression[i].symbol == ')')
		{
			while (stackEnd >= 0)
			{
				if (stack[stackEnd].tokenType == 1 && stack[stackEnd].symbol == '(')
					break;
				result[(*resultLength)++] = stack[stackEnd--];
			}
			if (stackEnd == -1)
				return NULL; // Invalid brackets
			stackEnd--; // Remove '('
		}
		else
		{
			int thisOpPriority = getOperatorPriority(expression[i].symbol);

			while (stackEnd >= 0 && (
				(getOperatorAssociativity(stack[stackEnd].symbol) == 1 && thisOpPriority <= getOperatorPriority(stack[stackEnd].symbol))
				||
				(getOperatorAssociativity(stack[stackEnd].symbol) == -1 && thisOpPriority < getOperatorPriority(stack[stackEnd].symbol))
				))
			{
				result[(*resultLength)++] = stack[stackEnd--];
			}
			stack[++stackEnd] = expression[i];
		}
	}

	while (stackEnd >= 0)
	{
		if (stack[stackEnd].tokenType == 1 && stack[stackEnd].symbol == '(')
			return NULL; // Invalid brackets
		result[(*resultLength)++] = stack[stackEnd--];
	}

	free(stack);
	return result;
}

char* calculateRPN(token_t *rpn, int rpnLength)
{
	double *stack = malloc(sizeof(double) * rpnLength);
	int stackEnd = -1;

	for (int i = 0; i < rpnLength; i++)
	{
		if (rpn[i].tokenType == 0)
		{
			stack[++stackEnd] = rpn[i].value;
		}
		else
		{
			if (stackEnd < 1) return "syntax error";
			if (rpn[i].symbol == '/' && stack[stackEnd] == 0) return "division by zero";
			double result = calculateOperator(rpn[i].symbol, stack[stackEnd--], stack[stackEnd--]);
			stack[++stackEnd] = result;
		}
	}

	if (stackEnd != 0)
	{
		free(stack);
		return "syntax error";
	}
	else
	{
		char *result = malloc(sizeof(char)*100);
		sprintf_s(result, 100, "%lf", stack[0]);
		free(stack);
		return result;
	}
}

void main()
{
	char expression[1001];
	gets_s(expression, 1001);

	int tokensLength = -1;
	token_t *tokens = parse(expression, &tokensLength);
	if (tokens != NULL)
	{
		int rpnLength = -1;
		token_t *rpn = convertToRPN(tokens, tokensLength, &rpnLength);
		if (rpn != NULL)
		{
			printf_s("%s", calculateRPN(rpn, rpnLength));
		}
		else
		{
			printf_s("syntax error");
		}
		free(rpn);
	}
	else
	{
		printf_s("syntax error");
	}

	free(tokens);
}