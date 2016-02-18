#pragma once

#include <stdlib.h>

#include "bit_sequence.h"

int bit_sequence_get_length(Bit_sequence * code)
{
	int result = 0;
	while (code != NULL)
	{
		code = code->next;
		result++;
	}
	return result;
}

void bit_sequence_free(Bit_sequence * code)
{
	if (code != NULL)
	{
		Bit_sequence * codePtr = code;
		if (code->next != NULL) bit_sequence_free(code->next);
		free(codePtr);
	}
}

Bit_sequence * bit_sequence_from_char(unsigned char c)
{
	Bit_sequence * result = NULL;
	unsigned char mask = 1 << 7;
	for (size_t i = 0; i < 8; i++)
	{
		bit_sequence_append(&result, !!(c & mask));
		mask >>= 1;
	}
	return result;
}

unsigned char bit_sequence_pop_char(Bit_sequence ** code)
{
	unsigned char result = 0;
	for (size_t i = 0; i < 8; i++)
	{
		result <<= 1;
		result |= (*code)->current_bit;
		bit_sequence_remove_first_bit(code);
	}
	return result;
}

void bit_sequence_append(Bit_sequence ** code, unsigned char bit)
{
	Bit_sequence * new_bit = malloc(sizeof(Bit_sequence));
	new_bit->current_bit = bit;
	new_bit->next = NULL;

	if (*code == NULL)
	{
		*code = new_bit;
	}
	else
	{
		Bit_sequence * codePtr = *code;
		while (codePtr->next != NULL)
		{
			codePtr = codePtr->next;
		}
		codePtr->next = new_bit;
	}
}

void bit_sequence_remove_last_bit(Bit_sequence ** code)
{
	if ((*code) == NULL)
	{ }
	else if ((*code)->next == NULL)
	{
		free(*code);
		(*code) = NULL;
	}
	else
	{
		Bit_sequence * codePtr = *code;
		while (codePtr->next->next != NULL)
		{
			codePtr = codePtr->next;
		}
		free(codePtr->next);
		codePtr->next = NULL;
	}
}

void bit_sequence_prepend(Bit_sequence * code, unsigned char bit)
{
	Bit_sequence * new_bit = malloc(sizeof(Bit_sequence));
	new_bit->current_bit = bit;
	new_bit->next = code;

	code = new_bit;
}

void bit_sequence_remove_first_bit(Bit_sequence ** code)
{
	if ((*code) == NULL)
	{
	}
	else if ((*code)->next == NULL)
	{
		free(*code);
		(*code) = NULL;
	}
	else
	{
		Bit_sequence * codePtr = *code;
		*code = (*code)->next;
		free(codePtr);
	}
}

Bit_sequence * bit_sequence_copy(Bit_sequence * code)
{
	if (code == NULL)
	{
		return NULL;
	}
	else
	{
		Bit_sequence * first = malloc(sizeof(Bit_sequence));
		Bit_sequence * current;

		first->current_bit = code->current_bit;
		current = first;

		while (code->next != NULL)
		{
			current->next = malloc(sizeof(Bit_sequence));
			current->next->current_bit = code->next->current_bit;

			code = code->next;
			current = current->next;
		}
		current->next = NULL;

		return first;
	}
}

Bit_sequence * bit_sequence_concat(Bit_sequence * a, Bit_sequence * b)
{
	Bit_sequence * newA = bit_sequence_copy(a);
	Bit_sequence * newB = bit_sequence_copy(b);

	if (a == NULL) return newB;
	else if (b == NULL) return newA;
	else
	{
		Bit_sequence * lastInA = newA;
		while (lastInA->next != NULL)
		{
			lastInA = lastInA->next;
		}
		lastInA->next = newB;

		return newA;
	}
}

void bit_sequence_append_sequence(Bit_sequence ** a, Bit_sequence * b)
{
	Bit_sequence * oldA = *a;
	*a = bit_sequence_concat(*a, b);
	bit_sequence_free(oldA);
}

void bit_sequence_save(Bit_sequence * a)
{
	unsigned char c = 0;
	int in_char_position = 0;
	do
	{
		c |= a->current_bit << (7-in_char_position);
		in_char_position++;
		if (in_char_position == 8)
		{
			printf("%c", c);
			c = 0;
			in_char_position = 0;
		}
		a = a->next;
	} while (a != NULL);

	if (in_char_position > 0)
	{
		printf("%c", c);
	}
}

Bit_sequence * bit_sequence_load(FILE * f)
{
	unsigned char byte, mask;
	int read = fread(&byte, sizeof(unsigned char), 1, f);
	Bit_sequence * result = NULL;

	while (read > 0)
	{
		mask = 1 << 7;
		for (int i = 0; i < 8; i++)
		{
			bit_sequence_append(&result, !!(byte & mask));
			mask >>= 1;
		}

		read = fread(&byte, sizeof(unsigned char), 1, f);
	}

	return result;
}