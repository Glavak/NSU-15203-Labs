#include "block.h"

Block * block_create(enum block_type type)
{
	Block * block = malloc(sizeof(Block));
	block->type = type;
	block->wave_index = -1;
	return block;
}

void block_print(Block * block)
{
	switch (block->type)
	{
	case WALL:
		printf(" W");
		break;
	case START:
		printf(" S");
		break;
	case FINISH:
		printf(" F");
		break;
	case PAVEMENT:
		printf(" P");
		break;
	default:
		if (block->wave_index < 0)
		{
			printf(" -");
		}
		else
		{
			printf("%2d", block->wave_index);
		}
		break;
	}
}

void block_set_wave_index(Block * block, int wave_index)
{
	if ((block->type == EMPTY || block->type == FINISH)
		&& block->wave_index == -1)
	{
		block->wave_index = wave_index;
	}
}

void block_free(Block * block)
{
	free(block);
}