// Exposed calls for GUI

#include "manager/addons_dict.h"
#include "maki/mktype.h" // for uintX
#include "pak/workshop_manager.h" // for download definitions

#define MAX_LEN_ADDONDIR_NAME 32
#define MAX_LEN_ADDONDIR_NAME 32

void
init_addonv (addonv* self)
{
	self->len = 0;
	self->push = addonv__push(addons_vector);
	self->pop = addonv__pop(addons_vector);
	return entries;
}

void
addonv__pop (addonv* self)
{
	--self->len;
}

void
addonv__push (addonv* self)
{
	++self->len;
}

void
addonv__test ()
{
	printf("Hi!\n");
}

void
load_order ()
{

}

void
load_info ()
{

}

void load_addons_iter ()
{

}

void startup ()
{
	// create dictionary
	addonv* addons_vector = (addonv*)malloc(sizeof(addonv));
	init_addonv(addons_vector);
	addons_vector->test();
}

void make_link ()
{
	
}

void import_addon (const char* vpk)
{
	// create an addon dir in addons
	// move vpk to addon dir
	// sync out some files
}

void merge_comfortable ()
{
	// merge headers to one, spread file data evenly.
}

void merge_compact ()
{
	// merge vpks into one. Only if their sizes allow it.
}

void check_nonutf8 ()
{
	
}

void
_get_basedir (char* buffer)
{

}

void
addon_prepare (const char* const addondir_name)
{
	uint8_t status = 0;
	char msg[64];
	msg[0] = '\0';
	/*
	i dont know how to avoid malloc. Like, memory on the stack cant
	cant be returned by function.
	Heck, the practice is to create a buffer before calling a function.
	*/
	char addoninfo_path[255+15+1];
	addoninfo_path[0]='\0';
	mk_strcpy (addoninfo_path, addondir_path);
	mk_strcat (addoninfo_path, "/addoninfo.json");
	cJSON* addoninfo_json = cJSON_Parse (addoninfo);
	if (addoninfo_json == NULL)
	{
		mk_strcpy (msg, "Error: couldn't find addoninfo.json\n");
		status = 1;
		goto end;
	}
	cJSON pkgs = cJSON_GetObjectItemCaseSensitive (addoninfo_json, "packages");
	cJSON_ArrayForEach (pkg, pkgs)
	{
		cJSON* fname = cJSON_GetObjectItemCaseSensitive (pkg,"file");
		char vpk_path[255+MAX_LEN_VPK_NAME+1];
		vpk_path[0]='\0';
		mk_strcat (vpk_path,fname->valuestring);
		cJSON* src = cJSON_GetObjectItemCaseSensitive (pkg,"source");
		cJSON* id = cJSON_GetObjectItemCaseSensitive (pkg,"id");
		dl_tket ticket = {src, id, fname->valuestring};
		dlqueue->push(dlqueue, &dl_tket);
	}

	end:
		cJSON_Delete (addoninfo);
		printf ("%s\n",msg);
		return status;
}

/*

void sync_headerfiles (const char* const pak_dir_path)
{
	char names[2][] = { "addoninfo", "addonimage" };
	char exts[2][3][] = { {"txt", '\0', '\0'}, {"jpg", "png", "vtf"} };
	for ( int i = 0; i < 2; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			if ( exts[i][j][0] == '\0' ) continue;
			char (*target_name)[20];
			*target_name = '\0';
			char *p = target_name;
			p = mk_strcat (p, names[i]);
			p = mk_strcat (p, ".");
			p = mk_strcat (p, exts[i][j]);
			unpack_single (target_name, pak_dir_path);
		}
	}
	// sync external addoninfo, addonimage and addon_dir with
	// their internal counterparts
}
*/