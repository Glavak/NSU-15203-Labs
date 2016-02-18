#pragma once

#include "item.h"

typedef struct priority_queue_element
{
	Item * item;
	struct priority_queue_element * next;
} Priority_queue_element;

typedef struct priority_queue
{
	Priority_queue_element * first;
	int count;
} Priority_queue;

Priority_queue * priority_queue_create(void);

Item * priority_queue_extract_minimum(Priority_queue * queue);

void priority_queue_insert(Priority_queue * queue, Item * item);