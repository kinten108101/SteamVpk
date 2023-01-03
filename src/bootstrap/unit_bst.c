#include <stdio.h>
#include "../smkl/io.h"
#include "../smkl/stack.h"
#include "../smkl/queue.h"
#include <stdlib.h>

typedef struct Node Node;
struct Node
{
	int key;
	Node *left;
	Node *right;
	Node *parent;
};

struct BST
{
	Node *root;
	size_t size;
	size_t height;
} BST;

void grow_tree (struct BST *tree, int key)
{
	Node *new_node = (Node *) malloc (sizeof (Node));
	
}

/**
 * 
 * [idx] key   left   right
 * 0     15    1      2 
 * 1     6     3      4
 * 2     18    5      6
 * 3     3     7      8
 * 4     7     N      9
 * 5     17    N      N
 * 6     20    N      N
 * 7     2     N      N
 * 8     4     N      N
 * 9     13    10     N
 * 10    9     N      N
 * 
 * 15, 6, 18, 3, 7, 17, 20, 2, 9, 7, 13
 */ 

Queue queue_lineup (char * string)
{
	Queue ret;
	return ret;
}

int
stack_is_empty (Stack self)
{
	return (stack_get_size (self) == 0 ? 1 : 0);
}

Node *
tree_get_child (Node * node)
{
	if (node->left) return node->left;
	else if (node->right) return node->right;
	else return NULL;
}

void inorder_walk_iterative(struct BST *tree)
{
	Stack buffer, printout;
	if (tree->root == NULL) return;
	stack_add (&buffer, tree->root);
	Node *top_tile_child;
	while (!stack_is_empty(buffer))
	{
		top_tile_child = tree_get_child ((Node *) buffer.top->content);
		if (!top_tile_child)
		{
			stack_add (&printout, stack_pop (&buffer));
			stack_add (&printout, stack_pop (&buffer));
			continue;
		}
		free ( stack_pop (&buffer) );
		stack_add (&buffer, top_tile_child->right);
		stack_add (&buffer, top_tile_child);
		stack_add (&buffer, top_tile_child->left);
	}
	stack_inorder_walk (&printout);
	return;
}

void inorder_walk_recursive (struct BST *tree)
{

}

BST
tree_seed (void)
{
	BST 
}

int main (int argc, char * argv[])
{
	BST mtree = tree_seed ();
	Queue waitlist = queue_lineup (argv[1]);
	waitlist.push (mtree.root);
	for (;;)
	{
		Node *left = tree_grow (waitlist.current);
		waitlist.push (left);
		Node *right = tree_grow (waitlist.current);
		waitlist.push (right);
		waitlist.pop ();
	}
}