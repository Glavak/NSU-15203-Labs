#pragma once

#include "Edge.h"

typedef struct priorityQueueElement
{
	int vertexIndex;
	struct priorityQueueElement * next;
} PriorityQueueElement;

typedef struct priorityQueue
{
	PriorityQueueElement * first;
	int count;
} PriorityQueue;

PriorityQueue * priority_queue_create(void);

void priority_queue_remove(PriorityQueue * queue, int index);
void priority_queue_extract_minimum(PriorityQueue * queue, int * index);
void priority_queue_insert(PriorityQueue * queue, int index, Edge * tags);

int priority_queue_is_empty(PriorityQueue * queue);