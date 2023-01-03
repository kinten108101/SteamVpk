#ifndef MV_DOWNLOAD_DLQUEUE_H
#define MV_DOWNLOAD_DLQUEUE_H

#include "../smkl/type.h"
#include <curl/curl.h>

enum DlSrc
{
	LOCAL,
	STEAMWORKSHOP,
	GAMEMAPS 
};

typedef struct DlTicket DlTicket;
struct DlTicket
{
	const char source  : 3;
	const uint64_t id  : 8;
	const char *dir;
	const uint8_t flag : 3;
	
	/**
	 * 
	 * X X X X X INFO IMG FILE
	 * 1: file only
	 * 2: img only
	 * 3: file and image
	 * 4: info only
	 * 7: file, img and info
	 * 
	 */

	DlTicket *next;
};

typedef struct
{
	size_t size;
	DlTicket *front, *rear;
	CURL *curl_handle;
} DlQueue;

extern int      dlqueue_push       (DlQueue *, DlTicket *);
extern int      dlqueue_sift       (DlQueue *);
extern int      dlqueue_pop        (DlQueue *, size_t);
extern int      dlqueue_steamcmd_dl_all (DlQueue *, const char * const);
extern int      dlqueue_handle     (DlQueue *);
extern int      dlqueue_printrear  (DlQueue *);
extern int      dlqueue_printfront (DlQueue *);
extern DlQueue *dlqueue_init       ();
extern int      dlqueue_close      (DlQueue *);

#endif