// Any read/write vpk archives

#include <stdio.h>
#include "pak.h" // for pakheader struct
#define SIZE_HEADER 12
#define SIZE_VPKENTRY 18
// does define work across source files?
unsigned int
get_tree_size (FILE* f)
{
	unsigned int tree_size;
	fseek(f,8,SEEK_SET);
	fread(&tree_size, 1, 4, f);
	return tree_size;
}

void
unpack_single (const char* target_file,
               const char* pak_dir_path)
{
	FILE* f = fopen(pak_dir_path,"rb");
	unsigned int tree_size = _get_tree_size(f);
	fseek(f,SIZE_HEADER + tree_size,SEEK_SET);
	while (1)
	{
		char ext [SIZE_FEXT];
		uint8 len_ext = _read_string(f,ext);
		if (strcmp(ext,"")==0) break;
		while (1)
		{
			char dir [SIZE_FDIR];
			uint8 len_dir = _read_string(f,dir);
			if (strcmp(dir,"")==0) break;
			while (1)
			{
				char name [SIZE_FNAME];
				uint8 len_name = _read_string(f,name);
				if (strcmp(name,"")==0) break;
				char path_buff[len_ext+len_dir+len_name+1];
				path_buff[0] = '\0';
				char* path_ptr = path_buff;
				path_ptr = maki_strcat(path_ptr,dir);
				path_ptr = maki_strcat(path_ptr,name);
				path_ptr = maki_strcat(path_ptr,ext);
				if (strcmp(target_file,path_buff)==0)
				{
					// TODO: You dont have to rewrite this 3layered loop
					// all the time. Implement delegate or sth.
					PakEntry entry;
					fread(&entry,1,SIZE_VPKENTRY,f);
					char* content = (char*)malloc(1*entry.entry_size);
					long prev = ftell(f);
					fseek(f,SIZE_HEADER + tree_size + entry.entry_offset,SEEK_SET);
					fread(content,1,size,f);
					fseek(f,prev,SEEK_SET);
					char* out_path = directory_of_ (pak_dir_path);
					FILE* out = fopen(out_path,"wb");
					if (out == NULL)
					{
						exit(1);
					}
					fwrite(content,1,entry.entry_size,out);
				}
				else fseek(f,SIZE_VPKENTRY,SEEK_CUR);
			}
		}
	}
	fclose(f); free(f);
}