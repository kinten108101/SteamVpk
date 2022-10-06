#include <stdio.h> // Promise that it is temporary
#include <unistd.h>

// Universal (legacy) log
void
mk_ulog(const char* message, const char flag)
{
	char icon[5] = "(?:)";
	switch (flag) {
		case 'I': 
			icon[1] = ':';
			break;
		case 'E':
			icon[1] = 'E';
			// icon
			break;
		default:
			printf("This should not happen!"); break; // throw exception
	}
	// TODO: pattern matcher substitution
	printf("%s %s\n",icon,message);
}

void
format_string ()
{

}

void
mkerr (const char* message)
{
}

void
mk_mkdir (const char* path)
{
	char* dirs = (char*)malloc(strlen(path)+1);
	char* dirs_start = dirs;

	while (*dirs = *path)
	{
		if ( *dirs == '/' )
		{
			*(++dirs)='\0'; dirs--;
			mkdir(dirs_start,0755);
		}
		dirs++;
		path++;
	}
	free(dirs);
}

void
mkcp (const char* f_from,
	    const char* f_to)
{
}