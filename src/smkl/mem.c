#include <stdlib.h>

void *
mk_malloc (int size)
{
	return malloc (size);
}

void *
mk_realloc (void *last_mem_loc, int size)
{
	return realloc (last_mem_loc, size);
}

void
mk_free (void *mem)
{
	return free (mem);
}