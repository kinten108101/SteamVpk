/**
 *
 * The Standard Maki-Koji Library is a reimplementation of C's
 * Standard Library. It borrows only unistd.h to access syscalls
 * (thus it currently supports only UNIX-like operating systems)
 *
 */

#include <stdio.h> // Promise that it is temporary
#include <unistd.h>
#include "string.h"
#include <sys/stat.h>

// Universal (legacy) log
int
mk_ulog(const char* message, const char flag)
{
	int ret = -1;
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
			printf("This should not happen!");
			ret = 1;
			goto end; // throw exception
	}
	/**
   *
   *  TODO: pattern-matching substitution
   *
   */
	printf("%s %s\n",icon,message);
	ret = 0;
end:
	return ret;
}

int
mkerr (const char *const message)
{
	int ret = mk_ulog(message,'E');
	return ret;
}

void
mk_mkdir (const char* path)
{
	char* dirs = (char*)malloc(mk_strlen(path)+1);
	char* dirs_start = dirs;

	while ((*dirs = *path))
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
mkdie (const char* msg)
{
	fputs ("(E:) ", stderr);
	fputs (msg, stderr);
	fputc ('\n', stderr);
	exit (1);
}

void
mkwarn (const char* msg)
{
	fputs ("(W:) ", stdout);
	fputs (msg, stdout);
	fputc ('\n', stdout);
}