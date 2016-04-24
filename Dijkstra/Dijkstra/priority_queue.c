#pragma once

#include <stdlib.h>

#include "priority_queue.h"

PriorityQueue * priority_queue_create(void)
{
	PriorityQueue * queue = malloc(sizeof(PriorityQueue));
	queue->first = NULL;
	queue->count = 0;
	return queue;
}

void priority_queue_extract_minimum(PriorityQueue * queue, int * index)
{
	if (queue == NULL || queue->first == NULL) return;
	else
	{
		// Take first element & item
		PriorityQueueElement * first_element = queue->first;
		*index = first_element->vertexIndex;

		// Pop it from queue
		queue->first = first_element->next;
		queue->count--;
		free(first_element);
	}
}

void priority_queue_remove(PriorityQueue * queue, int index)
{
	if (queue->first == NULL) return;

	if (queue->first->vertexIndex == index)
	{
		PriorityQueueElement * first_element = queue->first;
		queue->first = first_element->next;
		queue->count--;
		free(first_element);
		return;
	}

	if (queue->first->next == NULL) return;

	PriorityQueueElement * current_element = queue->first;
	while (current_element->next->vertexIndex != index)
	{
		current_element = current_element->next;
		if (current_element->next == NULL) return;
	}


	PriorityQueueElement * next_element = current_element->next;

	current_element->next = next_element->next;
	queue->count--;
	free(next_element);
}

void priority_queue_insert(PriorityQueue * queue, int index, Edge * tags)
{
	PriorityQueueElement * new_element = malloc(sizeof(PriorityQueueElement));
	new_element->vertexIndex = index;

	if (queue->first == NULL)
	{
		new_element->next = NULL;
		queue->first = new_element;
	}
	else if (edge_compare(tags[queue->first->vertexIndex], tags[index]) <= 0)
	{
		new_element->next = queue->first;
		queue->first = new_element;
	}
	else
	{
		// Find element, after which we gonna insert @item
		PriorityQueueElement * current_element = queue->first;
		while (current_element->next != NULL && edge_compare(tags[current_element->next->vertexIndex], tags[index]) > 0)
		{
			current_element = current_element->next;
		}

		// Insert it
		new_element->next = current_element->next;
		current_element->next = new_element;
	}

	queue->count++;
}

int priority_queue_is_empty(PriorityQueue * queue)
{
	return queue->first == NULL;
}