#pragma once

#include <stdlib.h>
#include <assert.h>

#include "bit_sequence.h"

Bit_sequence * bit_sequence_create(int capacity)
{
	assert(capacity % 8 == 0);

	Bit_sequence * sequence = malloc(sizeof(Bit_sequence));
	sequence->bytes = malloc(sizeof(unsigned char) * (capacity / 8));
	sequence->bits_capacity = capacity;
	sequence->bits_count = 0;
	sequence->bits_start = 0;
	return sequence;
}

int bit_sequence_get_length(Bit_sequence * code)
{
	return code->bits_count - code->bits_start;
}

void bit_sequence_free(Bit_sequence * code)
{
	free(code->bytes);
	free(code);
}

void bit_sequence_append_char(Bit_sequence * code, unsigned char c)
{
	// TODO: can be optimized with only two shifts
	unsigned char mask = 1 << 7;
	for (size_t i = 0; i < 8; i++)
	{
		bit_sequence_append(code, !!(c & mask));
		mask >>= 1;
	}
}

unsigned char bit_sequence_pop_char(Bit_sequence * code)
{
	// TODO: can be optimized with only two shifts
	unsigned char result = 0;
	for (size_t i = 0; i < 8; i++)
	{
		result <<= 1;
		result |= bit_sequence_get_first_bit(code);
		bit_sequence_remove_first_bit(code);
	}
	return result;
}

unsigned char bit_sequence_get_first_bit(Bit_sequence * code)
{
	unsigned char byte = code->bytes[code->bits_start / 8];
	unsigned char bit = (byte >> (7-code->bits_start % 8)) & 1;
	return bit;
}

void bit_sequence_append(Bit_sequence * code, unsigned char bit)
{
	assert(code->bits_count + 1 <= code->bits_capacity);

	if (bit)
	{
		code->bytes[code->bits_count / 8] |= 1 << (7-code->bits_count % 8);
	}
	else
	{
		code->bytes[code->bits_count / 8] &= ~(1 << (7-code->bits_count % 8));
	}

	code->bits_count++;
}

void bit_sequence_remove_last_bit(Bit_sequence * code)
{
	code->bits_count--;
}

void bit_sequence_remove_first_bit(Bit_sequence * code)
{
	code->bits_start++;
}

Bit_sequence * bit_sequence_copy(Bit_sequence * code)
{
	Bit_sequence * copy = bit_sequence_create(code->bits_capacity);
	copy->bits_start = code->bits_start;
	copy->bits_count = code->bits_count;

	for (size_t i = code->bits_start / 8; i <= code->bits_count / 8; i++)
	{
		copy->bytes[i] = code->bytes[i];
	}

	return copy;
}

Bit_sequence * bit_sequence_concat(Bit_sequence * a, Bit_sequence * b)
{
	Bit_sequence * copy = bit_sequence_copy(a);
	bit_sequence_append_sequence(copy, b);
	return copy;
}

void bit_sequence_append_sequence(Bit_sequence * a, Bit_sequence * b)
{
	for (size_t i = b->bits_start; i < b->bits_count; i++)
	{
		unsigned char byte = b->bytes[i / 8];
		unsigned char bit = (byte >> (7-i % 8)) & 1;
		bit_sequence_append(a, bit);
	}
}

void bit_sequence_save_and_free(Bit_sequence * a, FILE * output)
{
	while (bit_sequence_get_length(a) > 0)
	{
		fprintf(output, "%c", bit_sequence_pop_char(a));
	}

	bit_sequence_free(a);
}

Bit_sequence * bit_sequence_load(FILE * f)
{
	const int realloc_by = 1024;
	Bit_sequence * sequence = bit_sequence_create(realloc_by);
	unsigned char ch;

	size_t read = fread(&ch, 1, 1, f);
	while (read > 0)
	{
		if (sequence->bits_count + 8 > sequence->bits_capacity)
		{
			sequence->bytes = realloc(sequence->bytes, sequence->bits_capacity + realloc_by);
			sequence->bits_capacity += realloc_by;
		}
		bit_sequence_append_char(sequence, ch);
		read = fread(&ch, 1, 1, f);
	}
	return sequence;
}

void bit_sequence_print(Bit_sequence * a)
{
	for (size_t i = a->bits_start; i < a->bits_count; i++)
	{
		unsigned char byte = a->bytes[i / 8];
		unsigned char bit = (byte >> (7 - i % 8)) & 1;
		printf("%i", bit);
	}
}