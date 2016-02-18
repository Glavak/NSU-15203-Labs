#pragma once

#include <stdio.h>

#include "bit_sequence.h"

typedef struct item
{
	char isItem;// 0 - if node, 1 - if item
	unsigned char symbol;
	int priority;
	struct item * left;
	struct item * right;
} Item;

Item * item_create_from_char(unsigned char c, int priority);

Item * item_create_from_items(Item * i1, Item * i2);

Bit_sequence * item_to_sequence(Item * item);

Item * item_from_sequence(Bit_sequence ** code);