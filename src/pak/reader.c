#include <stdio.h>
#include <stdlib.h>
#include "pak.h"

int main(int argc, char* argv[])
{	
	FILE* pfile = fopen(,"rb");
	if (!pfile) {
		fputs("Can't find file!\n",stderr);
		exit(1);
	}
	
	
	fclose(pfile);
	return 0;
}