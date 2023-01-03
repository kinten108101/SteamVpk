#include <curl/curl.h>
#include "../smkl/io.h"
#include "../smkl/mem.h"
#include <string.h>
#include "../download/curl.h"

static size_t
restxt_handler (void *dat,
				size_t chunksize, size_t size,
				void *userdata) /* writedata */
{
	size_t realsize = chunksize * size;
	RESTXT *txt = (RESTXT *) userdata; /* cast buffer to right size? */

	char *p = mk_realloc (txt->stream, txt->size + realsize + 1);
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

static inline int
_curl_set_download_html (CURL *handler, const char *url, RESTXT *txt)
{
	curl_easy_setopt (handler, CURLOPT_URL, url);
	curl_easy_setopt (handler, CURLOPT_WRITEFUNCTION, restxt_handler);
	curl_easy_setopt (handler, CURLOPT_WRITEDATA, (void *) txt);
	return 0;
}

static inline int
_curl_set_download_file (CURL *handler, const char *url, FILE *f)
{
	curl_easy_setopt (handler, CURLOPT_URL, url);
	curl_easy_setopt (handler, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_setopt (handler, CURLOPT_WRITEDATA, f);
	return 0;
}

int main()
{
	CURL *curl;
	curl = curl_easy_init ();
	RESTXT *txt;
	txt = restxt_init ();
	CURLcode ret;

	//curl_easy_setopt (curl, CURLOPT_URL, "https://api.steampowered.com/IPublishedFileService/GetDetails/v1/?key=30D870823524715EA9EDA066C7515094&includetags=true&includeadditionalpreviews=true&includechildren=true&includekvtags=true&includevotes=true&short_description=true&includeforsaledata=true&includemetadata=true&return_playtime_stats=0&appid=550&strip_description_bbcode=true&publishedfileids[0]=2874630852");
	_curl_set_download_html (
		curl,
		"https://api.steampowered.com/IPublishedFileService/GetDetails/v1/?key=30D870823524715EA9EDA066C7515094&includetags=true&includeadditionalpreviews=true&includechildren=true&includekvtags=true&includevotes=true&short_description=true&includeforsaledata=true&includemetadata=true&return_playtime_stats=0&appid=550&strip_description_bbcode=true&publishedfileids[0]=2874630852",
		txt);
	ret = curl_easy_perform (curl);
	if ( ret != (CURLcode) CURLE_OK )
	{
		mkerr("Failed to write to response buffer!");
		return 1;
	}
	printf ("Data is: %s\n", txt->stream);
	printf ("Size is: %lu\n", txt->size);

	FILE *file;
	file = fopen ("/home/kinten/Garage/mates-vpk/tmp/newfile", "wb");
	if (!file)
	{
		mkdie ("Can't create a file there!");
	}
	_curl_set_download_file (
		curl,
		"https://steamusercontent-a.akamaihd.net/ugc/1879715254263813322/8FDED84400B09DCC27EEE5302F90758019F075D2/",
		file
		);
	ret = curl_easy_perform (curl);
	if ( ret != (CURLcode) CURLE_OK )
	{
		mkerr("Failed to write to response buffer!");
		return 1;
	}
	fclose (file);
	curl_easy_cleanup (curl);
	restxt_clean (txt);
	return 0;
}