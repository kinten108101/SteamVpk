#include <stdio.h>
#include <stdlib.h> // for malloc and free
#include "../smkl/type.h"
// <> vs ""

/*

DESC: Shell scripts has no object paradigm. Numbers once reordered do
not carry with them the objects' other attributes.

Can't improvise from a sorted list. Shell script would have to post-
process, matching the numbers from Input with the same ones from Output.
That will require an additional algorithm, an overhead.

The current solution is to implement a cache index array that describes
where a number has moved to from its original position. The shell script
can follow this description to move other attributes accordingly.

EXAMPLE:

json='
"objects":[
{
	"name":"objectA",
	"pos":21,
},
{
	"name":"objectB",
	"pos":33,
},
{
	"name":"objectC",
	"pos":14,
}
]'
	
Input:			21,33,14
Output:			14,21,33

if cacheIndex mode is enabled:
Output (-l):	0>1|1>2|2>0
Compressed (-k):1,2,0

(Input[0], which is 21, has moved to Output[1];
Input[1], which is 33, has moved to Output[2];
Input[2], which is 21, has moved to Output[0].)

So we can do this:

	input="21,33,14"
	new="21X,33Y,14Z"
	output_cidx=$(sort -k 21,33,14)
	for i in 0 1 2; do
		j=$( cut -d',' -f"$((${i}+1))" <<< "${output_cidx}" )
		new_j=$( cut -d ',' -f"${j}" <<< "${new}" )
		json=$( jq .objects[i].pos=new_j <<< "${json}" )
	done

TODO: Better example
TODO: Reimplement this in bitmap instead of int linked list.
TODO: Correct output should be 0>1|1>2|2>0, ordered by cacheIdx

*/

static inline void
swap (int* A, int* B)
{
	int tmp = *A;
	*A=*B;
	*B=tmp;
}

void _mergesort(int max, int start, int* arr, int* cIdx) {
	if ( max > 2 )
	{
		int maxA = max/2, startA = start;
		int maxB = max-max/2, startB = start+max/2;
		_mergesort(maxA, startA, arr, cIdx);
		_mergesort(maxB, startB, arr, cIdx);
		int i=startA, j=startB;
		int *tmp_arr = (int*)malloc(sizeof(int)*max), m=0;
		int *tmp_cIdx = (int*)malloc(sizeof(int)*max);
		int k=0; 
		while (k < max)
		{
			if (arr[i] < arr[j])
			{
				tmp_arr[m]=arr[i];
				tmp_cIdx[m]=cIdx[i];
				i++;
			}
			else 
			{ 
				tmp_arr[m]=arr[j];
				tmp_cIdx[m]=cIdx[j];
				j++;
			}
			m++;
			if ( j == startB + maxB ) { j--; arr[j] = arr[startA + maxA - 1] +1; }
			else if ( i == startA + maxA ) { i--; arr[i] = arr[startB + maxB - 1] +1; }
			// Add 1. If two values are equal then the j route above will always be chosen. That messes up the cIdx flow.
			k++;
		}

		// For some reasons, I typed k<=max instead of k<max.
		// Buffer overflow, but no crash.
		// Well, it will crash if there are six number elements speficially??
		// But it won't crash in above case if there's stdout before sort_merge()??
		// Better question, you can assign values to unallocated chunks? I thought that was illegal.
		i=start; m=0;
		while (m<max)
		{
			arr[i]=tmp_arr[m];
			cIdx[i]=tmp_cIdx[m];
			i++;m++;
		}
		free(tmp_arr);
		free(tmp_cIdx);
	}
	else if (max == 2)
	{
		if (arr[start] > arr[start+1])
		{ 
			// No need for tmp because it's a binary operation
			swap(&arr[start],&arr[start+1]); 
			swap(&cIdx[start],&cIdx[start+1]);
		}
	}
}

void
sort_merge(int arrC, int * arr, int mode, int null)
{
	// TODO: Bitflag options!
	//printf("Raw:\t");
	//for (int i = 0; i < arrC; i++) printf("%d,",arr[i]);
	//printf("\n");

	int* cacheIdx = (int*)malloc(sizeof(int)*arrC);
	for (int i = 0; i<arrC;i++) cacheIdx[i]=i;
	_mergesort(arrC,0,arr, cacheIdx);
	
	switch (mode)
	{
		case 0:
			for (int i = 0; i < arrC - 1; i++) printf("%d,",arr[i]);
			printf("%d",arr[arrC-1]);
			if (!null) printf("%c",'\n');
			else printf("%c",'\0');
			break;
		case 1:
			for (int i = 0; i < arrC - 1; i++) printf("%d,",cacheIdx[i]);
			printf("%d",cacheIdx[arrC-1]);
			if (!null) printf("%c",'\n');
			else printf("%c",'\0');
			break;
		case 2:
			for (int i = 0; i < arrC - 1; i++) printf("%d>%d|",cacheIdx[i],i);
			printf("%d>%d",cacheIdx[arrC-1],arrC-1);
			if (!null) printf("%c",'\n');
			else printf("%c",'\0');
			break;
	}
	free(cacheIdx);
	return;
}

void
sort_bubble(int * arr)
{
	int *i, *j;
	uint16_t swap_calls;
	do
	{
		swap_calls = 0;
		i = arr;
		j = arr; j++;
		while (*j)
		{
			if (*i > *j)
			{
				swap(i,j);
				swap_calls++;
			}
			i++; j++;
		}
	} while (swap_calls > 0);
}

#define MAX_ARR_LEN 255

int main(int argc, char* argv[]) {
	int opt_sort_type = 1;
	int opt_cache_index = 0;
	int opt_null = 0;
	
	if (argc < 2) 
	{
		printf(
			"Usage: %s [OPTIONS] 'A,B,C,D,...'\n"
			"OPTIONS =\n"
			"-h	Display this menu\n"
			"-t	Specify an algorithm\n"
			"1= Merge Sort (default)\n"
			"2= Bubble Sort\n"
			"3= Bucket Sort\n"
			"-k	Output changes of indices\n"
			"-l	Above but human-readable\n"
			, argv[0]);
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
	char *str = argv[optind];
	uint16_t narr[MAX_ARR_LEN];
	uint16_t narrC;
	narrC = kj_cstr2iarr(&narr,',',str);

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