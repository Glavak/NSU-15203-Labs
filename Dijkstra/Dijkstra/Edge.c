#pragma once

#include "Edge.h"

#include <stdlib.h>
#include <stdio.h>

Edge edge_defined_length(int length)
{
	Edge e;
	e.length = length;
	e.isInfinite = 0;
	e.isOverflowed = 0;
	return e;
}

Edge edge_ininite()
{
	Edge e;
	e.isInfinite = 1;
	e.isOverflowed = 0;
	return e;
}

Edge edge_owerflowed()
{
	Edge e;
	e.isInfinite = 0;
	e.isOverflowed = 1;
	return e;
}

int edge_compare(Edge a, Edge b)
{
	if (a.isInfinite && b.isInfinite) return 0;
	else if (a.isInfinite) return -1;
	else if (b.isInfinite) return 1;

	if (a.isOverflowed && b.isOverflowed) return 0;
	else if (a.isOverflowed) return -1;
	else if (b.isOverflowed) return 1;

	return b.length - a.length;
}

Edge edge_summ(Edge a, Edge b)
{
	Edge result;

	result.isInfinite = a.isInfinite || b.isInfinite;
	result.isOverflowed = a.isOverflowed || b.isOverflowed;
	
	if (!result.isInfinite && !result.isOverflowed &&
		(a.length > 0 && b.length > INT_MAX - a.length))
		result.isOverflowed = 1;
	else
		result.length = a.length + b.length;

	return result;
}

char * edge_to_string(Edge edge)
{
	if (edge.isInfinite) return "oo";
	if (edge.isOverflowed) return "INT_MAX+";

	char * a = malloc(sizeof(char) * 15);
	sprintf_s(a, 15, "%d", edge.length);
	return a;
}