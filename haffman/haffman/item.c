#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "item.h"
#include "bit_sequence.h"

Item * item_create_from_char(unsigned char c, int priority)
{
	Item * i = malloc(sizeof(Item));
	i->isItem = 1;
	i->symbol = c;
	i->priority = priority;
	i->left = NULL;
	i->right = NULL;
	return i;
}

Item * item_create_from_items(Item * i1, Item * i2)
{
	Item * i = malloc(sizeof(Item));
	i->isItem = 0;
	i->priority = i1->priority + i2->priority;
	i->left = i1;
	i->right = i2;
	return i;
}

Bit_sequence * item_to_sequence(Item * item)
{
	Bit_sequence * result = NULL;

	if (item->isItem)
	{
		// Mark that it's item, and write it
		bit_sequence_append(&result, 0);
		bit_sequence_append_sequence(&result, bit_sequence_from_char(item->symbol));
	}
	else
	{
		// Mark that it's node, and write it's children
		bit_sequence_append(&result, 1);
		bit_sequence_append_sequence(&result, item_to_sequence(item->left));
		bit_sequence_append_sequence(&result, item_to_sequence(item->right));
	}

	return result;
}

Item * item_from_sequence(Bit_sequence ** code)
{
	Item * new_item = malloc(sizeof(Item));

	if ((*code)->current_bit == 1)
	{
		*code = (*code)->next;
		new_item->isItem = 0;
		new_item->priority = 0;
		new_item->left = item_from_sequence(code);
		new_item->right = item_from_sequence(code);
	}
	else
	{
		*code = (*code)->next;
		new_item->isItem = 1;
		new_item->symbol = bit_sequence_pop_char(code);
		new_item->priority = 0;
	}

	return new_item;
}