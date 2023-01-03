/**
 * Implementing the linked list data structure
 */

#include <stdio.h>
#include <unistd.h> /* for access() */
#include <stdlib.h> /* for malloc() */
#include "../vendors/cJSON.h"

typedef struct AddonInfo
{
	unsigned short int pos      :  9; /* max number is 511 */
	unsigned char en            :  1; /* C bool */
	char tags                   :  6; /* max is 63 */
	char name[64];
	char authors[8][32];
	char desc[1024]; /* no bit fields for pointers; but really pointers are for arrays, and why would you want to conserve space in this case? */
} AddonInfo;

typedef struct AddonNode AddonNode;
struct AddonNode
{
	/* info */
	AddonInfo info;
	// paklist_t * const paklist;
	/* next */
	AddonNode *next;
};

AddonNode *
AddonNode_ctor (AddonInfo info)
{
	AddonNode *node = (AddonNode *) malloc (sizeof (AddonNode));
	node->info = info;
	return node;
}

typedef struct AddonList
{
	unsigned short int len      :  9; /* max is 511 */
	AddonNode *head, *tail;
} AddonList;

AddonList *
addonlist_ctor ()
{
	AddonList *list = (AddonList *) malloc (sizeof (AddonList));
	list->len = 0;
	AddonNode *first_node = (AddonNode *) malloc (sizeof (AddonNode));
	list->head = first_node;
	list->tail = first_node;
	return list;
}

static void
node_dtor_iter (AddonNode *node)
{
	if (node->next != NULL)
		node_dtor_iter (node->next);
	free (node);
	return;
}

void
addonlist_dtor (AddonList *list)
{
	node_dtor_iter (list->head);
	free (list);
}

void
addonlist_add2head (AddonList *list,
	                AddonInfo info)
{
	AddonNode *new_node = AddonNode_ctor (info);
	list->head = new_node;
	list->len++;
}

void
addonlist_add2tail (AddonList *list,
	                AddonInfo info)
{
	AddonNode *new_node = AddonNode_ctor (info);
	list->tail->next = new_node;
	list->tail = new_node;
	list->tail->next = NULL;
	list->len++;
}

void
addonlist_delete (AddonList *list)
{
	if (!list->len) return;
}

AddonInfo
addonlist_getentry (AddonList *list, size_t idx_target)
{
	AddonNode *tmp = list->head;
	size_t i = 0;
	while (i != idx_target)
	{
		i++;
		tmp = tmp->next;
	}
	return tmp->info;
}

void
test_traverse (AddonList *list)
{
	AddonNode *cur_node = list->head;
	for (int i = 0; i < list->len; i++)
	{
		AddonInfo info = cur_node->info;
		
		cur_node = cur_node->next;
	}
}

int
main ()
{
}