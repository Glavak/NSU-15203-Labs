#pragma once

#include <stdlib.h>

typedef int ContentType;

typedef struct tree_node 
{
	ContentType data;
	struct tree_node * left;
	struct tree_node * right;
} Tree_node;

typedef enum { LEFT, RIGHT } Direction;


Tree_node * tree_create(ContentType content);

size_t tree_get_height(Tree_node * root);

void tree_free(Tree_node * root);

Tree_node * tree_insert_raw(Tree_node * root, ContentType content, Direction direction);

void tree_delete_node(Tree_node * root, Direction direction);

Tree_node * tree_insert_balanced(Tree_node * root, ContentType content);

Tree_node * tree_find(Tree_node * root, ContentType data);