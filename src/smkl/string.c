#include "type.h"
#include "math.h"
#include <stdlib.h> /* for malloc() and free() */
#include <string.h>

int mk_strcmp (const char* s1, const char* s2)
{
	return strcmp(s1, s2);
}

unsigned long
mk_atoi (char *src)
{
  unsigned long ret = 0;
  int sign = 1;
  if (*src == '-')
  {
    sign = -1;
    src++;
  }
  while (*src)
  {
    ret = ret*10 + sign*((*src++) - '0');
  }
  return ret;
}

char *
mk_strread (char * dest, const char * src, char delimiter)
{
	while (*dest) dest++;
	while (*src != delimiter)
	{
		*dest++ = *src++;
	}
	*dest = '\0';
	return dest;
}

char *
mk_strcat (      char *dest,
           const char *src)
{
	return mk_strread (dest, src, '\0');
}

uint16_t
mk_strlen (const char* str)
{
  unsigned short int i = 0;
  while (*str)
  {
    str++;
    i++;
  }
  return i;
}

#define WLEN_MAX 10

unsigned short
kj_cstr2iarr (int* buffer,
               char delim,
               char* src)
{
    char* w = (char*)malloc(WLEN_MAX);
    char* const wlen_ptr = w; w++; // first byte is reserved for word length
    unsigned short buffer_len = 0;
    // TODO: buffer_len as buffer[0];
    while (1)
    {
        if (*src == delim || *src == '\0') {
            if ( *wlen_ptr > WLEN_MAX) exit(1);
            *buffer=0;
            buffer_len+=(unsigned short)*wlen_ptr;
            //w-=(*wlen_ptr);
            w = wlen_ptr;
            w++;
            while (*wlen_ptr != 0)
            {
                *buffer += (*w - '0') * mk_pow(10,(*wlen_ptr)-1);
                w++;
                (*wlen_ptr)--;
            }
            w = wlen_ptr;
            w++;
            if ( *src == '\0' ) break;
            else {
                src++;
                buffer++;
                continue;
            }
        }
        *w=*src;
        w++;src++;
        (*wlen_ptr)++;
    }
    return buffer_len;
}

// Problem with pascal string: buffer cant simply be created
// with malloc. 

/*
void
kj_cstr2piarr (int* buff_ptr,
               char delim,
               char* cstr)
{
    unsigned short bufflen_tmp = 0;
    while (*cstr)
    {
        bufflen_tmp++;
        cstr++;
    }
    unsigned short* buff = (unsigned short)malloc(sizeof(unsigned short)*buff_len + 1);
    unsigned short* const bufflen_ptr = buff; buff++;
    *buff = bufflen_tmp;
    char* w = (char*)malloc(WLEN_MAX + 1);
    char* const wlen_ptr = w; w++;
    while (1)
    {
        if (*cstr == delim || *cstr == '\0') {
            if ( *wlen_ptr > WLEN_MAX) mkerr("A number exceeded max number of digits!");
            w = wlen_ptr;
            w++;
            *buffer=0;
            while (*wlen_ptr != 0)
            {
                *buff += (*w - '0') * maki_pow(10,(*wlen_ptr)-1);
                w++;
                (*wlen_ptr)--;
            }
            w = wlen_ptr;
            w++;
            if ( *cstr == '\0' ) break;
            else {
                cstr++;
                buffer++;
                continue;
            }
        }
        *w=*cstr;
        w++;cstr++;
        (*wlen_ptr)++;
    }
}

*/