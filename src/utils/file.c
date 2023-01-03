#include <stdio.h>
#include "cJSON.h"
#include "../smkl/string.h"
#include "../smkl/io.h"

#define PATH_SETTINGS "resources/settings.json"

#define RETURN_WITH_STATUS_CODE(value_status_code) result=value_status_code; goto exit;
#define ASSERT_FROM_LAST_STATUS(value_last_status) if (value_last_status != 0) result=value_last_status; goto exit;

int
load_text_file (char *buffer, const char *path_file)
{
	int result = 0;

	FILE *fd;
	int file_size;
	
	fd = fopen (path_file, "rb");
	if (!fd)
	{
		RETURN_WITH_STATUS_CODE (-1);
	}
	file_size = fseek (fd, 0, SEEK_END);
	if (file_size > 1024)
	{
		RETURN_WITH_STATUS_CODE (-1);
	}
	fread (buffer, 1, file_size, fd);

exit:
	if (fd) fclose (fd);
	return result;
}

int
load_program_settings (void)
{
	int result = 0;
	char settings_fstream[1024];
	cJSON *json_obj_root, *json_obj_path_addons;
	
	result = load_text_file (settings_fstream, PATH_SETTINGS);
	ASSERT_FROM_LAST_STATUS (result);
	json_obj_root = cJSON_Parse(settings_fstream);

exit:
	cJSON_Delete(json_obj_root);
	return result;
}

int
streamgets (char * const buffer, char *stream)
{
	int byte_read = 0;
	char *p = buffer;

	p = mk_strread (p, stream, '\n');
	byte_read = p - buffer;

	return byte_read;
}