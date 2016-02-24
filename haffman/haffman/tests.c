#include "tests.h"

void run_tests()
{
	Bit_sequence * a = bit_sequence_create(8);
	bit_sequence_append_char(a, 'a');

	assert(bit_sequence_get_length(a) == 8);
	assert(bit_sequence_pop_char(a) == 'a');
	assert(bit_sequence_get_length(a) == 0);
}