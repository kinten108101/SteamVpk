#include <stdio.h>
#include "cJSON.h"
#include "../smkl/string.h"
#include "io.h"

#define PATH_ADDONLIST "profile/default/addonlist.csv"

int linker_load ()
{
	int result = 0;
	char addonlist_fstream[1024];

	result = load_text_file (addonlist_fstream, PATH_ADDONLIST);
	while (1)
	{
		char entry[1024];
		streamgets (entry, addonlist_fstream);
		
	}
	/** 
	 * 
	 * 
	 * load csv
	 * for entry in entries:
	 *     if entry.en == 1:
	 * 			symlink(entry.path, game_addon_dir);
	 * 
	 */



exit:
	return result;
}