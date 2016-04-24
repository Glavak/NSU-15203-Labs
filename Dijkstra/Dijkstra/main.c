#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

void print_path(Edge * tags, Graph * graph, int from, int to)
{
	if (edge_is_infinite(tags[to]))
	{
		printf_s("no path");
		return;
	}
	if (edge_is_overflowed(tags[to]))
	{
		// Check if there are only one path

		int current = to;
		int nextCurrent;

		do
		{
			nextCurrent = -1;
			for (int i = 0; i < graph->vertexCount; i++)
			{
				if (current == i) continue;

				Edge edge = graph_get_edge_length(graph, current, i);
				if (!edge_is_infinite(edge) &&
					edge_compare(edge_summ(tags[i], edge), tags[current]) == 0)
				{
					if (nextCurrent == -1)
					{
						nextCurrent = i;
					}
					else
					{
						// this is second possible path
						printf_s("overflow");
						return;
					}
				}
			}
			current = nextCurrent;
		} while (current != from);

	}
	int current = to;
	do
	{
		printf_s("%d ", current + 1);

		for (int i = 0; i < graph->vertexCount; i++)
		{
			Edge edge = graph_get_edge_length(graph, current, i);
			if (!edge_is_infinite(edge) && !edge_is_overflowed(edge) &&
				edge_compare(edge_summ(tags[i], edge), tags[current]) == 0)
			{
				// If there is an edge between current and i,
				// and tag[i] + this edge == tag[current]
				current = i;
				break;
			}
		}
	} while (current != from);

	printf_s("%d", from + 1);
}

void main()
{
	int N;
	scanf_s("%d", &N);
	if (N < 0 || N > 5000)
	{
		printf_s("bad number of vertices");
		return;
	}

	int S, F;
	scanf_s("%d %d", &S, &F);
	if (S <= 0 || F <= 0 || S > N || F > N)
	{
		printf_s("bad vertex");
		return;
	}
	S--; F--;

	int M;
	scanf_s("%d", &M);
	if (M < 0 || M > N*(N - 1) / 2)
	{
		printf_s("bad number of edges");
		return;
	}

	Graph * graph = graph_create(N);

	int from, to, length;
	for (int i = 0; i < M; i++)
	{
		scanf_s("%d %d %d", &from, &to, &length);
		if (from <= 0 || to <= 0 || from > N || to > N)
		{
			printf_s("bad vertex");
			return;
		}
		if (length < 0)
		{
			printf_s("bad length");
			return;
		}

		graph_set_edge_length(graph, from - 1, to - 1, length);
	}

	Edge * lengths = malloc(sizeof(Edge)*N);
	graph_dijkstra_run(graph, S, lengths);

	// Print lengths to verticies
	for (int i = 0; i < N; i++)
	{
		printf_s("%s ", edge_to_string(lengths[i]));
	}

	printf_s("\n");

	print_path(lengths, graph, S, F);

	getchar();
}