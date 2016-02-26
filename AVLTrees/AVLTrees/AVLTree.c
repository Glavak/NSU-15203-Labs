#include "AVLTree.h"

Tree_node * tree_create(int key)
{
	Tree_node * node = malloc(sizeof(Tree_node));

	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;

	return node;
}

unsigned char tree_get_height(Tree_node * root)
{
	if (root == NULL) return 0;
	else return root->height;
}

int tree_get_balance_factor(Tree_node * root)
{
	return (int)tree_get_height(root->right) - (int)tree_get_height(root->left);
}

void tree_count_height(Tree_node * root)
{
	unsigned char hl = tree_get_height(root->left);
	unsigned char hr = tree_get_height(root->right);
	root->height = (hl > hr ? hl : hr) + 1;
}

Tree_node * tree_rotateright(Tree_node * p)
{
	Tree_node * q = p->left;
	p->left = q->right;
	q->right = p;
	tree_count_height(p);
	tree_count_height(q);
	return q;
}

Tree_node * tree_rotateleft(Tree_node * q)
{
	Tree_node * p = q->right;
	q->right = p->left;
	p->left = q;
	tree_count_height(q);
	tree_count_height(p);
	return p;
}

Tree_node * tree_balance(Tree_node * p)
{
	tree_count_height(p);
	if (tree_get_balance_factor(p) == 2)
	{
		if (tree_get_balance_factor(p->right) < 0)
			p->right = tree_rotateright(p->right);
		return tree_rotateleft(p);
	}
	if (tree_get_balance_factor(p) == -2)
	{
		if (tree_get_balance_factor(p->left) > 0)
			p->left = tree_rotateleft(p->left);
		return tree_rotateright(p);
	}
	return p;
}

Tree_node * tree_insert(Tree_node * p, int k)
{
	if (p == NULL) return tree_create(k);
	if (k<p->key)
		p->left = tree_insert(p->left, k);
	else
		p->right = tree_insert(p->right, k);
	return tree_balance(p);
}