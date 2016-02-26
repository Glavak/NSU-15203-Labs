#include <stdio.h>
#include <math.h>

#include "AVLTree.h"

void main(void)
{
	size_t N;
	scanf_s("%d", &N);

	if (N == 0)
	{
		printf("0");
	}
	else
	{
		int a;
		scanf_s("%d", &a);
		Tree_node * root = tree_create(a);
		for (size_t i = 1; i < N; i++)
		{
			scanf_s("%d", &a);
			root = tree_insert(root, a);
		}
		printf("%d", tree_get_height(root));
	}
}