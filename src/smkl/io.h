#ifndef SMKL_IO_H
#define SMKL_IO_H

#define NULL ((void *)0)
extern void	mk_ulog (const char *, const char);
extern void	mk_mkdir (const char *);
extern int mkerr (const char * const);

/* Exit program with exception message */
extern void mkdie (const char *message);
/* Print out a non-interruptive warning */
extern void mkwarn (const char *message);
// extern void mkcp (const char*, const char*);

#endif