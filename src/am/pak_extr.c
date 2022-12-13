#include <stdio.h> // for file operations
#include <stdlib.h> // for exit
#include <string.h> // for strcmp, strcat
#include <sys/stat.h> // for mkdir
#include "pak.h" // for struct definition (not declaration) ALSO DEPENDS stdio
#include "maki_string.h" // for maki_strcat, maki_strlen

#define SIZE_FEXT 8
#define SIZE_FDIR 64
#define SIZE_FNAME 32
#define SIZE_VPKHEADER 12
#define SIZE_VPKENTRY 18

static inline void
_read_string (FILE* f, char* str ) 
{
	char* cur = (char*) malloc (1);
	do
	{
		fread (cur,1,1,f);
		*(str++)=*cur;
	}
	while(*cur);
	*str='\0';
	free(cur);
}

void 
write_tree_iter	(const char* path,	
                 FILE* f,
                 unsigned int tree_size, 
                 unsigned int offset, 
                 unsigned int length)
{
	char* content = (char*) malloc (length);
	long prev_offset = ftell (f);

	fseek (f, SIZE_VPKHEADER + tree_size + offset, SEEK_SET);
	fread (content,1,length,f);
	fseek (f, prev_offset, SEEK_SET);
    
	mk_mkdir(path);
	FILE* f_out = fopen (path,"w");
	if (f_out==NULL)
	{
		fputs ("Something is wrong!\n", stderr);
		exit(1);
	}
	fwrite(content, 1, length, f_out);

	fclose(f_out);
	free(content);

}

void
_pathcat (char path[],
		  const char* base,
		  const char* dir,
		  const char* name,
		  const char* ext)
{
	path[0] = '\0';
	path = maki_strcat(path,base);

	path = maki_strcat(path,"/");
	if ( strcmp(dir," ") == 0 ) goto files;
	path = maki_strcat(path,dir);
	path = maki_strcat(path,"/");
	files :
	path = maki_strcat(path,name);
	path = maki_strcat(path,".");
	path = maki_strcat(path,ext);
}

char*
_pathcat_v2 (const char* base,
             const char* dir,
             const char* name,
             const char* ext)
{
	char* p = (char*)malloc(maki_strlen(base)+1
		+maki_strlen(dir)+1
		+maki_strlen(name)+1
		+maki_strlen(ext)
		+1);
	*p = '\0';
	char* start = p;
	p = maki_strcat(p,base);
	p = maki_strcat(p,"/");
	if ( strcmp(dir," ") == 0 ) goto files;
	p = maki_strcat(p,dir);
	p = maki_strcat(p,"/");
	files :
	p = maki_strcat(p,name);
	p = maki_strcat(p,".");
	p = maki_strcat(p,ext);
	return start;
}

void
write_tree (int size_tree, FILE* f) 
{
	while (1)
	{
		char ext [SIZE_FEXT];
		read_string(f,ext);
		if (strcmp(ext,"")==0) break;
		while (1)
		{
			char dir [SIZE_FDIR];
			read_string(f,dir);
			if (strcmp(dir,"")==0) break;
			while (1)
			{
				char name [SIZE_FNAME];
				read_string(f,name);	
				if (strcmp(name,"")==0) break;
			
				char path[200];
				_pathcat(path,basedir,dir,name,ext);
				
				PakEntry entry;
				fread(&entry,1,SIZE_VPKENTRY,f);
				write_tree_iter	(path,	
								f,
								size_tree,
								entry.entry_offset,
								entry.entry_size);
			}
		}
	}
}

PakHeader*
read_header (FILE* f)
{
	PakHeader* header = (PakHeader*)malloc(SIZE_VPKHEADER);
	fread(header,1,SIZE_VPKHEADER,f);
	// count vs step
	return header;
}

void
extract_pak (FILE* f)
{
	PakHeader* header = read_header(f);
	
	{
		printf("Singature: 0x%04x\n",header->sig);
		printf("ver: %d\n",header->ver);
		printf("Tree size (bytes): %d\n",header->tree_size);
	}
	write_tree(header->tree_size,f);
}