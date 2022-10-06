#ifndef ADDONS_DICT_H
#define ADDONS_DICT_H

typedef struct addonv_entry
{
	unsigned short int id; // 16 bit
	unsigned char en : 1; // 1 bit
	char* const name; // 8 bit
	char** const creators; // 8 bit
	unsigned long int tags : 50; // 50 bit
	unsigned short int priority : 9; // 9 bit -> max number is 512
	char* const archive_dir; // 8 bit
	unsigned char src : 3; // 3 bit // STEAMWORKSHOP | GAMEMAPS | LOCAL
	unsigned short int srcid; // 16 bit
	char* const desc; // 8 bit

} addonv_entry;

typedef struct addonv
{
	addonv_entry** entries;
	unsigned short int len;
	void (*push)(addonv* self);
	void (*pop)(addonv* self);
	void (*test)();
} addonv;
extern void init_addonv (addonv* addons_vector);
extern void addonv__push (addonv* self);
extern void addonv__pop (addonv* self);
extern void addonv__test ();

extern void startup();
#endif