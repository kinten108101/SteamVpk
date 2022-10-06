#include "pak/workshop_manager.h"

int main(int argc, char* argv[])
{
	dlqueue* queue;
	queue = init_dlqueue (queue);

	dltket ticket = {STEAMWORKSHOP, 2779276994, "nick_tlou2"};
	queue->push(queue, &ticket);
	printf("%d: %d | %ld | %s\n", queue->size, queue->rear->src, queue->rear->id, queue->rear->fname);

	dltket ticket1 = {GAMEMAPS, NULL, "xbox_hud"};
	queue->push(queue, &ticket1);
	printf("%d: %d | %ld | %s\n", queue->size, queue->rear->src, queue->rear->id, queue->rear->fname);

	dltket ticket2 = {LOCAL, 2714246914, "ram7_nstrey"};
	queue->push(queue, &ticket2);
	printf("%d: %d | %ld | %s\n", queue->size, queue->rear->src, queue->rear->id, queue->rear->fname);

	printf("%d: %d | %ld | %s\n", queue->size, queue->front->src, queue->front->id, queue->front->fname);
	queue->sift(queue);
	printf("%d: %d | %ld | %s\n", queue->size, queue->front->src, queue->front->id, queue->front->fname);
	queue->sift(queue);
	printf("%d: %d | %ld | %s\n", queue->size, queue->front->src, queue->front->id, queue->front->fname);
	printf("%d: %d | %ld | %s\n", queue->size, queue->rear->src, queue->rear->id, queue->rear->fname);
	queue->sift(queue);
	printf("%d\n", queue->size);
	dltket ticket3 = {LOCAL, 2302931945, "p90_kriegs"};
	queue->push(queue, &ticket3);
	printf("%d: %d | %ld | %s\n", queue->size, queue->rear->src, queue->rear->id, queue->rear->fname);
	
	close_dlqueue (queue);
}