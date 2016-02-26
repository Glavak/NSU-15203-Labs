#include <stdlib.h>

typedef struct tree_node
{
	int key;
	unsigned char height;
	struct tree_node * left;
	struct tree_node * right;
} Tree_node;

Tree_node * tree_create(int key);

unsigned char tree_get_height(Tree_node * root);

int tree_get_balance_factor(Tree_node * root);

void tree_count_height(Tree_node * root);

Tree_node * tree_rotateright(Tree_node * p);

Tree_node * tree_rotateleft(Tree_node * q);

Tree_node * tree_balance(Tree_node * p);

Tree_node * tree_insert(Tree_node * p, int k);