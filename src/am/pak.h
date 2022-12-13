#ifndef _PAK_H_
#define _PAK_H_

typedef struct PakHeader
{
	const unsigned int sig;
	const unsigned int ver;
	const unsigned int tree_size;
} PakHeader;

typedef struct PakEntry
{
	const unsigned int crc; // A 32bit crc of the file's data.
	const unsigned short preload_size; // The number of bytes contained in the index file.
	// A zero based index of the archive this file's data is contained in.
	// If 0x7fff, the data follows the f_dir.
	const unsigned short archive_idx;
	// If archive_idx is 0x7fff, the offset of the file data relative to the end of the f_dir (see the header for more details).
	// Otherwise, the offset of the data from the start of the specified archive.
	const unsigned int entry_offset;
	// If zero, the entire file is stored in the preload data.
	// Otherwise, the number of bytes stored starting at entry_offset.
	const unsigned int entry_size;
	const unsigned short Terminator;
} PakEntry;

typedef struct pakfile PakFile;
pakfile
{
	const char* ext;
	const char* name;
	const char* dir;
};

typedef struct hashlist
{
	// max 2^16=65536, whereas there are at most
	// 1000 files in a common vpk. We use MAX_FILES 2048
	unsigned short int len;
	unsigned short int** list;
	// byte size: 10 or 16 instead of 20 (stack overflow)
} hashlist;
// Pascal integer array. A string that starts with a len counter.
// Cost 4 more bytes but blazingly fast.

#endif