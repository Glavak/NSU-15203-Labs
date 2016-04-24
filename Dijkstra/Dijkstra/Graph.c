#pragma once

#include "Graph.h"
#include "priority_queue.h"

#include <stdlib.h>

Graph * graph_create(int vertexCount)
{
	Graph * graph = malloc(sizeof(Graph));
	graph->vertexCount = vertexCount;

	graph->edges = malloc(sizeof(Edge *) * vertexCount);
	for (int i = 0; i < vertexCount; i++)
	{
		graph->edges[i] = malloc(sizeof(Edge) * vertexCount);

		for (int j = 0; j < vertexCount; j++)
		{
			graph->edges[i][j] = edge_ininite();
		}
	}

	return graph;
}

void graph_set_edge_length(Graph * graph, int from, int to, int length)
{
	graph->edges[from][to] = edge_defined_length(length);
	graph->edges[to][from] = edge_defined_length(length);
}

Edge graph_get_edge_length(Graph * graph, int from, int to)
{
	return graph->edges[to][from];
}

char _is_all_vertices_visited(char * visited, int N)
{
	for (int i = 0; i < N; i++)
	{
		if (!visited[i]) return 0;
	}
	return 1;
}

// returns index of minimum tagged vertex, or -1 if they all visited
int _get_minimum_unvisited_vertex_number(Edge * tags, char * visited, int N)
{
	int minimum_number = -1;
	for (int i = 0; i < N; i++)
	{
		if (visited[i]) continue;

		if (minimum_number == -1)
		{
			minimum_number = i;
		}
		else if (edge_compare(tags[minimum_number], tags[i]) < 0)
		{
			minimum_number = i;
		}
	}
	return minimum_number;
}

void graph_dijkstra_run(Graph * graph, int from, Edge * lengths)
{
	char * visited = malloc(sizeof(char) * graph->vertexCount);

	// Set all vertex tags to infinity, and mark them as unvisited
	for (int i = 0; i < graph->vertexCount; i++)
	{
		lengths[i] = edge_ininite();
		visited[i] = 0;
	}
	lengths[from] = edge_defined_length(0);

	PriorityQueue * queue = priority_queue_create();
	priority_queue_insert(queue, from, lengths);
	
	while (!priority_queue_is_empty(queue))
	{
		int index;
		priority_queue_extract_minimum(queue, &index);
		Edge tag = lengths[index];

		if (edge_compare(tag, lengths[index]) < 0) continue;

		for (int i = 0; i < graph->vertexCount; i++)
		{
			Edge edge = graph_get_edge_length(graph, index, i);
			if (edge.isInfinite || edge.isOverflowed) continue;

			Edge new_tag = edge_summ(lengths[index], edge);
			if (edge_compare(new_tag, lengths[i]) > 0)
			{
				lengths[i] = new_tag;
				priority_queue_remove(queue, i);
				priority_queue_insert(queue, i, lengths);
			}
		}
	}
}

void graph_free(Graph * graph)
{
	for (int i = 0; i < graph->vertexCount; i++)
	{
		free(graph->edges[i]);
	}
	free(graph->edges);
	free(graph);
}