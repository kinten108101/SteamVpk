#include <stdlib.h> /* for size_t, malloc, realloc */
#include <string.h> /* for memcpy */
#include "../smkl/io.h" /* for mkerr */
#include "curl.h"

static size_t
restxt_handler (void *dat,
				size_t chunksize, size_t size,
				void *userdata) /* writedata */
{
	size_t realsize = chunksize * size;
	RESTXT *txt = (RESTXT *) userdata; /* cast buffer to right size? */

	char *p = realloc (txt->stream, txt->size + realsize + 1);
	if (!p)
	{
		mkdie ("Error at writing stream to response!");
		return 1;
	}

	txt->stream = p;
	memcpy (&(txt->stream[txt->size]), dat, realsize);
	txt->size += realsize;
	txt->stream[txt->size] = 0;
	return realsize;
}

int
restxt_clean (RESTXT *txt)
{
	free (txt->stream);
	free (txt);
	return 0;
}

RESTXT*
restxt_init ()
{
	RESTXT *txt = (RESTXT *) malloc (sizeof(RESTXT));
	txt->stream = (char *) malloc (1);
	txt->size = 0;
	return txt;
}

int
wmcurl_set_download_txt (CURL *handler, const char *url, RESTXT *txt)
{
	curl_easy_setopt (handler, CURLOPT_URL, url);
	curl_easy_setopt (handler, CURLOPT_WRITEFUNCTION, restxt_handler);
	curl_easy_setopt (handler, CURLOPT_WRITEDATA, (void *) txt);
	return 0;
}

int
wmcurl_set_download_file (CURL *handler, char * const url, FILE *f)
{
	curl_easy_setopt (handler, CURLOPT_URL, url);
	curl_easy_setopt (handler, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_setopt (handler, CURLOPT_WRITEDATA, f);
	return 0;
}