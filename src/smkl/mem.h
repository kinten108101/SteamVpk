#ifndef SMKL_MEM_H
#define SMKL_MEM_H

extern void * mk_malloc (int size);
extern void * mk_realloc (void * mem, int size);
extern void mk_free (void *mem);

#endif