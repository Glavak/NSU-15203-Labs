#pragma once

#include <stdlib.h>

#include "priority_queue.h"

Priority_queue * priority_queue_create(void)
{
	Priority_queue * queue = malloc(sizeof(Priority_queue));
	queue->first = NULL;
	queue->count = 0;
	return queue;
}

Item * priority_queue_extract_minimum(Priority_queue * queue)
{
	if (queue == NULL || queue->first == NULL) return NULL;
	else
	{
		// Take first element & item
		Priority_queue_element * first_element = queue->first;
		Item * first_item = first_element->item;

		// Pop it from queue
		queue->first = first_element->next;
		queue->count--;
		free(first_element);

		return first_item;
	}
}

void priority_queue_insert(Priority_queue * queue, Item * item)
{
	Priority_queue_element * new_element = malloc(sizeof(Priority_queue_element));
	new_element->item = item;

	if (queue->first == NULL)
	{
		new_element->next = NULL;
		queue->first = new_element;
	}
	else if (queue->first->item->priority >= item->priority)
	{
		new_element->next = queue->first;
		queue->first = new_element;
	}
	else
	{
		// Find element, after which we gonna insert @item
		Priority_queue_element * current_element = queue->first;
		while (current_element->next != NULL && current_element->next->item->priority < item->priority)
		{
			current_element = current_element->next;
		}

		// Insert it
		new_element->next = current_element->next;
		current_element->next = new_element;
	}

	queue->count++;
}