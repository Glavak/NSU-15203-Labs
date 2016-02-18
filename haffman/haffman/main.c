#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bit_sequence.h"
#include "item.h"
#include "priority_queue.h"

// Returns array of 256 ints, where array[i] is count of symbols (unsigned char)i
int * get_symbol_frequencies(unsigned char * string, size_t symbols_count)
{
	int * frequencies = malloc(sizeof(int)*256);
	for (int i = 0; i < 256; i++) frequencies[i] = 0;

	while (symbols_count > 0)
	{
		frequencies[*string]++;
		string++;
		symbols_count--;
	}

	return frequencies;
}

// Returns root element of Haffman tree for given input
Item * get_tree_root(unsigned char * input, size_t symbols_count)
{
	int * frequencies = get_symbol_frequencies(input, symbols_count);

	Priority_queue * queue = priority_queue_create();

	for (int i = 0; i < 256; i++)
	{
		if (frequencies[i] > 0)
		{
			priority_queue_insert(queue, item_create_from_char((unsigned char)i, frequencies[i]));
		}
	}

	while (queue->count > 1)
	{
		Item * new_item = item_create_from_items(priority_queue_extract_minimum(queue), priority_queue_extract_minimum(queue));
		priority_queue_insert(queue, new_item);
	}

	return priority_queue_extract_minimum(queue);
}

void build_codes_from_tree_helper(Item * tree_root, Bit_sequence * pre_code, Bit_sequence ** codes)
{
	if (tree_root->isItem)
	{
		codes[tree_root->symbol] = bit_sequence_copy(pre_code);
	}

	if (tree_root->left != NULL)
	{
		bit_sequence_append(&pre_code, 1);
		build_codes_from_tree_helper(tree_root->left, pre_code, codes);
		bit_sequence_remove_last_bit(&pre_code);
	}
	if (tree_root->right != NULL)
	{
		bit_sequence_append(&pre_code, 0);
		build_codes_from_tree_helper(tree_root->right, pre_code, codes);
		bit_sequence_remove_last_bit(&pre_code);
	}
}

// Returns array of 256 symbol codes, where array[i] is code for symbol (unsigned char)i
Bit_sequence ** build_codes_from_tree(Item * tree_root)
{
	Bit_sequence ** result = malloc(sizeof(Bit_sequence *)*256);
	for (size_t i = 0; i < 256; i++) result[i] = NULL;

	build_codes_from_tree_helper(tree_root, NULL, result);
	return result;
}

unsigned char * read_string(FILE * f, size_t * count)
{
	const int read_by = 1024;
	unsigned char * result = malloc(sizeof(unsigned char) * (read_by + 1));
	size_t position = 0;

	size_t read = fread(result, 1, read_by, f);
	while (read > 0)
	{
		position += read;
		result = realloc(result, position + read_by + 1);
		read = fread(result + position, 1, read_by, f);
	}

	*count = position;
	return result;
}

void debug_print_item(Item * a, int deep)
{
	for (int i = 0; i < deep; i++)		printf("+");

	if (a->symbol != 0)printf("%c\n", a->symbol);
	else printf("<na>\n");

	if (a->left != NULL)debug_print_item(a->left, deep + 1);
	if (a->right != NULL)debug_print_item(a->right, deep + 1);
}

void debug_print_code(Bit_sequence ** a)
{
	for (size_t i = 0; i < 256; i++)
	{
		if (a[i] != NULL)
		{
			Bit_sequence * current = a[i];
			do
			{
				printf("%i", current->current_bit);
				current = current->next;
			} while (current != NULL);
			printf(" - %c", i);
			printf("\n");
		}
	}
}

unsigned char decode_symbol(Bit_sequence ** code, Item * tree)
{
	if (tree->isItem)
	{
		return tree->symbol;
	}
	else
	{
		if ((*code)->current_bit == 1)
		{
			bit_sequence_remove_first_bit(code);
			return decode_symbol(code, tree->left);
		}
		else
		{
			bit_sequence_remove_first_bit(code);
			return decode_symbol(code, tree->right);
		}
	}
}

int is_text_contain_only_symbol(unsigned char * text, size_t symbols_count)
{
	unsigned char symbol = text[0];
	text++;
	symbols_count--;

	while (symbols_count > 0)
	{
		if (*text != symbol) return 0;
		text++;
		symbols_count--;
	}
	return 1;
}

void compress(FILE * f)
{
	size_t symbols_count;
	unsigned char * text = read_string(f, &symbols_count);

	// Don't try to compress empty string
	if (symbols_count == 0) return;

	Bit_sequence * result = NULL;

	// Fix for files with single byte repeated some times
	if (is_text_contain_only_symbol(text, symbols_count))
	{
		bit_sequence_append(&result, 0);
		bit_sequence_append_sequence(&result, bit_sequence_from_char(text[0]));

		bit_sequence_append_sequence(&result, bit_sequence_from_char((char)symbols_count));
	}
	else
	{
		Item * tree = get_tree_root(text, symbols_count);

		Bit_sequence ** codes = build_codes_from_tree(tree);

		// Serialize tree and compressed text
		Bit_sequence * tree_serialized = item_to_sequence(tree);
		Bit_sequence * text_serialized = NULL;
		while (symbols_count > 0)
		{
			bit_sequence_append_sequence(&text_serialized, codes[*text]);
			text++;
			symbols_count--;
		}

		// Insert separator
		int total_length = bit_sequence_get_length(tree_serialized) + bit_sequence_get_length(text_serialized) + 1;// + 1 is for 1-bit after next "while"
		while (total_length % 8 != 0)
		{
			bit_sequence_append(&tree_serialized, 0);
			total_length++;
		}
		bit_sequence_append(&tree_serialized, 1);

		result = bit_sequence_concat(tree_serialized, text_serialized);
		bit_sequence_free(tree_serialized);
		bit_sequence_free(text_serialized);
	}

	bit_sequence_save(result);
	bit_sequence_free(result);
}

void decompress(FILE * f)
{
	Bit_sequence * bits = bit_sequence_load(f);

	// Don't try to decompress empty data
	if (bits == NULL) return;

	// If the tree contain single element
	if (bits->current_bit == 0)
	{
		bit_sequence_remove_first_bit(&bits);
		unsigned char symbol = bit_sequence_pop_char(&bits);
		unsigned char count = bit_sequence_pop_char(&bits);

		for (char i = 0; i < count; i++) printf("%c", symbol);

		return;
	}
	else
	{
		Item * tree = item_from_sequence(&bits);

		// Skip separator
		while (bits->current_bit == 0) bit_sequence_remove_first_bit(&bits);
		bit_sequence_remove_first_bit(&bits);

		while (bits != NULL)
		{
			printf("%c", decode_symbol(&bits, tree));
		}
	}
}

int main(void)
{
	unsigned char mode;
	scanf_s("%c\n", &mode);

	if (mode == 'c')
	{
		// Compressing
		printf("d\n");
		compress(stdin, mode);
	}
	else if (mode == 'd')
	{
		// Decompressing
		decompress(stdin);
	}
}

