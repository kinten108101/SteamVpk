#include <stdio.h>
#include "../smkl/string.h"
#include <stdlib.h>

void test_four ()
{
	int arra[5];
	int *arrb = (int *) malloc (sizeof (int));
	int *arrc;
	printf ("%d\n", arra[2]);
	printf ("%d\n", *arrb);
	printf ("%d\n", *arrc);
}

void test_three ()
{
	char * const path = (char *) malloc (100);
	*path='\0';
	char *p = path;
	mk_strcat (p, "dls/");
	mk_strcat (p, "mw19_renetti");
	mk_strcat (p, ".vpk");
	printf ("%s\n", path);
	free (path);
}

void test_two ()
{
	char * const path = (char *) malloc (500);
	char *p = path;
	*p = '\0';
	p = mk_strcat (p, "https://api.steampowered.com/IPublishedFileService/GetDetails/v1/?key=30D870823524715EA9EDA066C7515094&includetags=true&includeadditionalpreviews=true&includechildren=true&includekvtags=true&includevotes=true&short_description=true&includeforsaledata=true&includemetadata=true&return_playtime_stats=0&appid=550&strip_description_bbcode=true&publishedfileids[0]=");
	p = mk_strcat (p, "1232423");
	printf ("%s\n",path);
	free (path);
}

void test_one ()
{
	char path[100];
	char *p = path;
	path[0]='\0';
	p = mk_strcat (p, "Hello");
	p = mk_strcat (p, " World!");
	printf ("%s\n",&path[0]);
}

int main ()
{
	test_four();
	return 0;
}
