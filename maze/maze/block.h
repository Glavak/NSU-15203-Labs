#pragma once

#include <stdlib.h>
#include <stdio.h>

enum block_type { WALL, EMPTY, START, FINISH, PAVEMENT };

typedef struct block
{
	enum block_type type;
	int wave_index;
} Block;

Block * block_create(enum block_type type);

void block_print(Block * block);

void block_set_wave_index(Block * block, int wave_index);

void block_free(Block * block);