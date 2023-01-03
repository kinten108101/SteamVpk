#include <curl/curl.h>
#include "../smkl/io.h"
#include "../smkl/mem.h"
#include "../smkl/type.h"
#include "dlqueue.h"
#include "curl.h"

#define LEN_URL_PUBLISHEDFILE 65

int
dlqueue_push (DlQueue *queue, DlTicket *new)
{
	queue->rear->next = new;
	queue->rear = new;
	queue->rear->next = NULL;
	return 0;
}

/* Initialize a Download Queue */
DlQueue *
dlqueue_init ()
{
	DlQueue *queue = (DlQueue *) mk_malloc (sizeof(DlQueue));
	queue->size = 0;
	return queue;
}