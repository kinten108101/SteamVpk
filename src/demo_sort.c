#include <stdio.h> // temporary i swear
#include <stdlib.h> // temporary i swear
#include "maki/mkio.h" // for ulog
#include "maki/mktype.h" // for uint16
#include "koji/kjstring.h" // for atoiarr
#include "koji/kjsort.h" // for sorting algorithms

#include <unistd.h> // for getopt
#include <getopt.h> // fix from stack overflow, apparently c99 unistd doesnt include getopt

#define MAX_ARR_LEN 255

void print_help()
{
printf("\
U sage: sort [OPTIONS] 'A,B,C,D,...'\n\
	OPTIONS =\n\
	-h	Display this menu\n\
	-t	Specify an algorithm\n\
		1= Merge Sort (default)\n\
		2= Bubble Sort\n\
		3= Bucket Sort\n\
	-k	Output changes of indices\n\
	-l	Above but human-readable\n\
");
}

int main(int argc, char* argv[]) {
	int opt_sort_type = 1;
	int opt_cache_index = 0;
	int opt_null = 0;
	
	if (argc < 2) 
	{
		print_help();
		return 1;
	}
	char c;
	while ( (c = getopt(argc, argv, "t:hklz")) != -1 )
	{
		switch(c)
		{
			case 't':
				opt_sort_type=(*optarg-'0'); break;
			case 'h':
				print_help(); return 1; break;
			case 'k':
				opt_cache_index = 1; break;
			case 'l':
				opt_cache_index = 2; break;
			case 'z':
				opt_null = 1; break;
			default:
				abort();
		}
	}
	char* str = argv[optind];
	uint16* narr = (int*)malloc(sizeof(uint16)*MAX_ARR_LEN);
	uint16 narrC;
	narrC = kj_cstr2iarr(narr,',',str);

	if ( narrC < 2 )
	{
		mk_ulog("Must be more than two elements!!",'E');
		return 1;
	}

	switch (opt_sort_type)
	{
		case 1:
			sort_merge(narrC,narr,opt_cache_index,opt_null); break;
		case 2:
			sort_bubble(narr); break;
		default:
			mk_ulog("Unimplemented sorting algorithm!",'E'); break;
	}
	
	free(narr);
	return 0;
}