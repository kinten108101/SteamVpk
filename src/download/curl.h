#ifndef CURL_H
#define CURL_H

#include <stdio.h>
#include <curl/curl.h>

typedef struct RESTXT
{
	char *stream;
	size_t size;
} RESTXT;

//extern size_t  restxt_handler (void *dat, size_t chunksize, size_t size, void *userp);
extern int     restxt_clean (RESTXT *res);
extern RESTXT* restxt_init ();
extern int wmcurl_set_download_txt  (CURL *handler, const char *url, RESTXT *res);
extern int wmcurl_set_download_file (CURL *handler, char * const url, FILE *f);

#endif