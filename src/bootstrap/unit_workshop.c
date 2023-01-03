#include <stdio.h>
#include "../download/dlqueue.h"
#include "../smkl/string.h"

static DlQueue *queue;

static void test_one ()
{
	extern DlQueue *queue;
	DlTicket ticket_0 = {STEAMWORKSHOP, 2888955831, "HD_Smoother_Reload", 3};
	dlqueue_push (queue, &ticket_0);
	DlTicket ticket_1 = {STEAMWORKSHOP, 2888008208, "mw19_renetti", 3};
	dlqueue_push (queue, &ticket_1);
	dlqueue_handle (queue);
	dlqueue_sift (queue);
	dlqueue_handle (queue);
	dlqueue_sift (queue);
}

int main (int argc, char *argv[])
{
	/* curl service */
	if (argc != 2) return 0;
	CURL *curl;
	curl = curl_easy_init ();
	/* download queue service */
	extern DlQueue *queue;
	queue = dlqueue_init ();
	dlqueue_connect_curl (queue, curl);
	/* body */
	DlTicket ticket = {STEAMWORKSHOP, mk_atoi (argv[1]), "sdf"};
	dlqueue_push (queue, &ticket);
	dlqueue_handle (queue);
	/* end */
	dlqueue_close (queue);
	curl_easy_cleanup (curl);
}