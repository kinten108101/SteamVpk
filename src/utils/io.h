#ifndef _MODEL_IO_H_
#define _MODEL_IO_H_

struct Settings
{
	char *path_to_game;
	char *path_to_game_addons;
};

extern int load_text_file (char *buffer, const char *path_file);
extern int load_program_settings (void);
extern int streamgets(char * const, char *);

#endif