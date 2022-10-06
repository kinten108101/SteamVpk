#include "workshop_manager.h"
#include <stdlib.h> // for realloc
#include <string.h> // for memcpy

static void
dlqueue__push (dlqueue* self, const dltket* new_tket)
{
	++self->size;
	self->tkets = (dltket*) realloc (self->tkets, sizeof(*new_tket) * self->size);
	self->front = self->tkets;
	self->rear = self->front + (self->size - 1);
	memcpy (self->rear, new_tket, sizeof(*new_tket));
}

static void
dlqueue__sift (dlqueue* self)
{
	--self->size;
	memmove(self->tkets, self->tkets + 1, sizeof(dltket)*self->size);
	self->tkets = (dltket*) realloc (self->tkets, sizeof(dltket) * self->size);
	self->front = self->tkets;
	self->rear = self->front + (self->size - 1);
}

static void
dlqueue__shrink (dlqueue* self)
{
}

dlqueue*
init_dlqueue (dlqueue* queue)
{
	queue = (dlqueue*) malloc (sizeof(dlqueue));
	queue->size = 0;
	queue->push = dlqueue__push;
	queue->sift = dlqueue__sift;
	queue->shrink = dlqueue__shrink;
	return queue;
}

void
close_dlqueue (dlqueue* queue)
{
	if (queue->tkets)
	{
		free (queue->tkets);
	}
	free(queue);
}

