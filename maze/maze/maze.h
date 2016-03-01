#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "block.h"

typedef struct maze
{
	Block *** blocks;
	size_t width;
	size_t height;
} Maze;

Maze * maze_create_empty(size_t width, size_t height);

void maze_display(Maze * maze);

void maze_fill_random(Maze * maze);

void maze_wave(Maze * maze);

void maze_build_pavement(Maze * maze);

void maze_free(Maze * maze);