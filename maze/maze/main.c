#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "block.h"
#include "maze.h"

const size_t maze_width = 10;
const size_t maze_height = 10;

void main(void)
{
	srand((unsigned int)time(NULL));

	char a = '\0';
	while (a!='x')
	{
		system("cls");
		printf("Generating maze:\n");
		Maze * maze = maze_create_empty(maze_width, maze_height);
		maze_fill_random(maze);
		maze_display(maze);

		printf("Applying wave algorithm:\n");
		maze_wave(maze);
		maze_display(maze);

		printf("Building path:\n");
		maze_build_pavement(maze);
		maze_display(maze);

		maze_free(maze);
		a = (char)getchar();
	}
}