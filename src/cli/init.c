#include <stdio.h> // for printf
#include <stdlib.h> // for exit
#include <unistd.h> // for getopt
#include "addons_dict.h"

void
demo_sort ()
{
	
}

void
print_help ()
{
	printf("usage:\n");
}

int
main (int argc, char* const argv[])
{
	while (argc == 1) {
		print_help();
		exit(0);
	}
	while ( (int c=getopt(argc,argv,"+")) != -1 )
	{
		switch(c)
		{
			case a:
				demo_sort();
				break;
			default:
				break;
		}
	}
}