#pragma once

typedef struct edge
{
	int length;
	char isInfinite;
	char isOverflowed;
} Edge;

Edge edge_defined_length(int length);
Edge edge_ininite();
Edge edge_owerflowed();

// returns positive number if a < b, negative if b < a, 0 if a==b
int edge_compare(Edge a, Edge b);
Edge edge_summ(Edge a, Edge b);

char * edge_to_string(Edge);
