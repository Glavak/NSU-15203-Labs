#include "Maze.h"

Maze * maze_create_empty(size_t width, size_t height)
{
	Maze * maze = malloc(sizeof(Maze));

	maze->blocks = malloc(sizeof(Block **) * height);
	for (size_t y = 0; y < height; y++)
	{
		maze->blocks[y] = malloc(sizeof(Block *) * width);
		for (size_t x = 0; x < width; x++)
		{
			maze->blocks[y][x] = block_create(EMPTY);
		}
	}

	maze->width = width;
	maze->height = height;
	return maze;
}

void maze_display(Maze * maze)
{
	printf("------------------------\n");
	for (size_t x = 0; x < maze->width; x++)
	{
		for (size_t y = 0; y < maze->height; y++)
		{
			block_print(maze->blocks[y][x]);
		}
		printf("\n");
	}
	printf("------------------------\n");
}

void maze_fill_random(Maze * maze)
{
	for (size_t y = 0; y < maze->height; y++)
	{
		for (size_t x = 0; x < maze->width; x++)
		{
			if (x == 0 || y == 0 || x == maze->width - 1 || y == maze->height - 1)
			{
				maze->blocks[y][x]->type = WALL;
			}
			else
			{
				maze->blocks[y][x]->type = rand() % 2 ? WALL : EMPTY;
			}
		}
	}

	int startX = 1 + rand() % (maze->width - 2);
	int startY = 1 + rand() % (maze->height - 2);
	maze->blocks[startY][startX]->type = START;

	int finishX = 1 + rand() % (maze->width - 2);
	int finishY = 1 + rand() % (maze->height - 2);
	maze->blocks[finishY][finishX]->type = FINISH;
}

void maze_wave(Maze * maze)
{
	int startX = -1;
	int startY = -1;

	// Find start block:
	for (size_t x = 0; x < maze->width && startX == -1; x++)
	{
		for (size_t y = 0; y < maze->height && startX == -1; y++)
		{
			if (maze->blocks[y][x]->type == START)
			{
				startX = x;
				startY = y;
			}
		}
	}

	if (startX == -1) return; // No start block

	// Start algorithm:
	maze->blocks[startY][startX]->wave_index = 0;

	int current_wave_index = 0;
	while (current_wave_index < (int)(maze->width + maze->height))
	{
		for (size_t x = 0; x < maze->width; x++)
		{
			for (size_t y = 0; y < maze->height; y++)
			{
				if (maze->blocks[y][x]->type == FINISH && maze->blocks[y][x]->wave_index == current_wave_index)
				{
					printf("Wave have reached finish, %d steps required\n", current_wave_index);
					return;
				}
				else if (maze->blocks[y][x]->type != WALL && maze->blocks[y][x]->wave_index == current_wave_index)
				{
					block_set_wave_index(maze->blocks[y + 1][x], current_wave_index + 1);
					block_set_wave_index(maze->blocks[y - 1][x], current_wave_index + 1);
					block_set_wave_index(maze->blocks[y][x + 1], current_wave_index + 1);
					block_set_wave_index(maze->blocks[y][x - 1], current_wave_index + 1);
				}
			}
		}

		current_wave_index++;
	}

	printf("Wave haven't reached finish, in isn't connected to start\n");
}

int _check_neigbour(Maze * maze, int * x, int * y, int offset_x, int offset_y)
{
	if (maze->blocks[*y + offset_y][*x + offset_x]->wave_index ==
		maze->blocks[*y][*x]->wave_index - 1)
	{
		*x += offset_x;
		*y += offset_y;
		if (maze->blocks[*y][*x]->type == EMPTY)
		{
			maze->blocks[*y][*x]->type = PAVEMENT;
		}
		return 1;
	}
	return 0;
}

void maze_build_pavement(Maze * maze)
{
	int finishX = -1;
	int finishY = -1;

	// Find start block:
	for (size_t x = 0; x < maze->width && finishX == -1; x++)
	{
		for (size_t y = 0; y < maze->height && finishX == -1; y++)
		{
			if (maze->blocks[y][x]->type == FINISH)
			{
				finishX = x;
				finishY = y;
			}
		}
	}

	if (finishX == -1) return; // No finish block

	if (maze->blocks[finishY][finishX]->wave_index == -1) return; // Finish block isn't connected to start

	// Move back to start:
	while (maze->blocks[finishY][finishX]->type != START)
	{
		if (_check_neigbour(maze, &finishX, &finishY, 1, 0)) continue;
		if (_check_neigbour(maze, &finishX, &finishY, -1, 0)) continue;
		if (_check_neigbour(maze, &finishX, &finishY, 0, 1)) continue;
		if (_check_neigbour(maze, &finishX, &finishY, 0, -1)) continue;
		return;
	}
}

void maze_free(Maze * maze)
{
	for (size_t y = 0; y < maze->height; y++)
	{
		for (size_t x = 0; x < maze->width; x++)
		{
			block_free(maze->blocks[y][x]);
		}
		free(maze->blocks[y]);
	}
	free(maze->blocks);
	free(maze);
}