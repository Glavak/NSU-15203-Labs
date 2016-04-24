#pragma once

typedef struct edge
{
	int length;
} Edge;

Edge edge_defined_length(int length);
Edge edge_infinite();
Edge edge_owerflowed();

int edge_is_infinite(Edge e);
int edge_is_overflowed(Edge e);

// returns positive number if a < b, negative if b < a, 0 if a==b
int edge_compare(Edge a, Edge b);
Edge edge_summ(Edge a, Edge b);

char * edge_to_string(Edge);
