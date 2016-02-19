#pragma once

#include <stdio.h>

typedef struct bit_sequence
{
	unsigned char current_bit;
	struct bit_sequence * next;
} Bit_sequence;


int bit_sequence_get_length(Bit_sequence * code);

void bit_sequence_free(Bit_sequence * code);

Bit_sequence * bit_sequence_from_char(unsigned char c);

unsigned char bit_sequence_pop_char(Bit_sequence ** code);

void bit_sequence_append(Bit_sequence ** code, unsigned char bit);

void bit_sequence_remove_last_bit(Bit_sequence ** code);

void bit_sequence_prepend(Bit_sequence * code, unsigned char bit);

void bit_sequence_remove_first_bit(Bit_sequence ** code);

Bit_sequence * bit_sequence_copy(Bit_sequence * code);

Bit_sequence * bit_sequence_concat(Bit_sequence * a, Bit_sequence * b);

void bit_sequence_append_sequence(Bit_sequence ** a, Bit_sequence * b);

void bit_sequence_save(Bit_sequence * a, FILE * output);

Bit_sequence * bit_sequence_load(FILE * f);