#include "tests.h"

int tests = 0;
int passed = 0;

void my_assert(int expression)
{
	printf("Test #%3i: %s\n", tests++, expression ? "passed" : "failed");
	if (expression) passed++;
}

void my_assert_conclude()
{
	printf("%i/%i tests passed", passed, tests);
}

void run_tests(void)
{
	Tree_node * tree = tree_create(42);
	Tree_node * node = tree_insert_balanced(tree, 50);
	my_assert(tree_find(tree, 50) == node);
	
	Tree_node * node2 = tree_insert_balanced(tree, 30);
	my_assert(tree_get_height(tree) == 2);
	
	tree_insert_raw(node2, 5, LEFT);
	my_assert(tree->left->left->data == 5);
	
	tree_insert_balanced(tree, 30);
	my_assert(tree->left->left->data == 5);

	tree_delete_node(tree, LEFT);
	my_assert(tree->left == NULL);
	my_assert(tree_get_height(tree) == 2);

	my_assert_conclude();
}
