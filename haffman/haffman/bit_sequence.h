#pragma once

#include <stdio.h>

typedef struct bit_sequence
{
	unsigned char * bytes;
	size_t bits_capacity; // maximum array capacity
	size_t bits_count; // count of bits in sequence (index of last bit + 1)
	size_t bits_start; // index of first bit in the array
} Bit_sequence;

Bit_sequence * bit_sequence_create(int capacity);

int bit_sequence_get_length(Bit_sequence * code);

void bit_sequence_free(Bit_sequence * code);

void bit_sequence_append_char(Bit_sequence * code, unsigned char c);

unsigned char bit_sequence_pop_char(Bit_sequence * code);

unsigned char bit_sequence_get_first_bit(Bit_sequence * code);

void bit_sequence_append(Bit_sequence * code, unsigned char bit);

void bit_sequence_remove_last_bit(Bit_sequence * code);

void bit_sequence_remove_first_bit(Bit_sequence * code);

Bit_sequence * bit_sequence_copy(Bit_sequence * code);

Bit_sequence * bit_sequence_concat(Bit_sequence * a, Bit_sequence * b);

void bit_sequence_append_sequence(Bit_sequence * a, Bit_sequence * b);

void bit_sequence_save_and_free(Bit_sequence * a, FILE * output);

Bit_sequence * bit_sequence_load(FILE * f);

void bit_sequence_print(Bit_sequence * a);