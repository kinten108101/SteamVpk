#ifndef SMKL_STRING_H
#define SMKL_STRING_H
#include "type.h" // for uint16

/* TODO */
extern int mk_strcmp (const char *s1, const char *s2);

/* Convert an array of number characters to integer */
extern unsigned long mk_atoi (char *);

/**
 * Concatenate a string segment (source) to another string (destination). Segment is defined by the substring from start to the first specified delimiter.
 * Return the pointer to the end of the concatenated string. In concatenate chaining, this can be used as dest pointer in the next call.
 */
extern char*	mk_strread(char* dest, const char* src, char delimiter);

/**
 * Concatenate a string (source) to another (destination). Under the hood, this is mk_strread but with delimiter set to '\0'.
 * Return the pointer to the end of the concatenated string. In concatenate chaining, this can be used as dest pointer in the next call.
 */
extern char*	mk_strcat(char* dest, const char* src);
extern int		mk_strlen(const char* str);
extern char*	mk_getstdin();
extern uint16_t itrn_atoiarr (int* buffer, char delim, char* src);

#endif