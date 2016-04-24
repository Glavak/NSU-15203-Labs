#pragma once

#include "Edge.h"

#include <stdlib.h>
#include <stdio.h>

Edge edge_defined_length(int length)
{
	Edge e;
	e.length = length;
	return e;
}

Edge edge_infinite()
{
	Edge e;
	e.length = -1;
	return e;
}

Edge edge_owerflowed()
{
	Edge e;
	e.length = -2;
	return e;
}

int edge_is_infinite(Edge e)
{
	return e.length == -1;
}

int edge_is_overflowed(Edge e)
{
	return e.length == -2;
}

int edge_compare(Edge a, Edge b)
{
	if (edge_is_infinite(a) && edge_is_infinite(b)) return 0;
	else if (edge_is_infinite(a)) return -1;
	else if (edge_is_infinite(b)) return 1;

	if (edge_is_overflowed(a) && edge_is_overflowed(b)) return 0;
	else if (edge_is_overflowed(a)) return -1;
	else if (edge_is_overflowed(b)) return 1;

	return b.length - a.length;
}

Edge edge_summ(Edge a, Edge b)
{
	if (edge_is_infinite(a) || edge_is_infinite(b)) return edge_infinite();

	if (edge_is_overflowed(a) || edge_is_overflowed(b)) return edge_owerflowed();
	
	if ((a.length > 0 && b.length > INT_MAX - a.length))
		return edge_owerflowed();
	else
		return edge_defined_length(a.length + b.length);
}

char * edge_to_string(Edge edge)
{
	if (edge_is_infinite(edge)) return "oo";
	if (edge_is_overflowed(edge)) return "INT_MAX+";

	char * a = malloc(sizeof(char) * 15);
	sprintf_s(a, 15, "%d", edge.length);
	return a;
}