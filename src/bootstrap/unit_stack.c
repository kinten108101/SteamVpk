#include "../smkl/io.h"
#include "../smkl/type.h"

typedef struct Tile Tile;
struct Tile
{
	void *content;
	Tile *prev;
};

Tile *
tile_create (void *content)
{
	Tile *ret_tile = (Tile *) malloc (sizeof (Tile));
	ret_tile->content = content;
	return ret_tile;
}

typedef struct Stack
{
	Tile *top, *bottom;
	size_t size;
} Stack;

void stack_add (Stack *self, void *obj)
{
	struct Info
	{
		int a;
		char b;
	};
	struct Info *info = (struct Info *) malloc (sizeof (struct Info));
	Tile *new_tile = tile_create (info);
	new_tile->content = obj;
	new_tile->prev = self->top;
}

void *stack_pop (Stack *self)
{
	if (self->size < 1)
		mkdie ("Stack has reached popping limit!");
	Tile *tmp = self->top->prev;
	Tile *ret = self->top;
	self->size--;
	self->top = tmp;
	return ret;
}