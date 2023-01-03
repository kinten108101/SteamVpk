#include "../smkl/mem.h"
#include "../smkl/io.h"
#include <stdio.h>

/***** TICKET *****/
typedef struct Ticket Ticket;
struct Ticket
{
    int a;
    char b;
    Ticket *next;
};

Ticket *
ticket_new (int a, char b)
{
    Ticket *guest = (Ticket *) mk_malloc ( sizeof (Ticket));
    guest->a = a;
    guest->b = b;
    guest->next = NULL;
    return guest;
}

/***** QUEUE *****/
typedef struct Queue
{
    int size;
    Ticket *head, *rear;
} Queue;

Queue *
queue_lineup (void)
{
    Queue *queue = (Queue *) mk_malloc ( sizeof (Queue));
    queue->size = 0;
    return queue;
}

void
queue_free_ticket_free_recur (Ticket *ticket)
{
    if (ticket->next)
        queue_free_ticket_free_recur (ticket->next);
    mk_free (ticket);
    return;
}

void
queue_free (Queue *self)
{
    queue_free_ticket_free_recur (self->head);
    mk_free (self);
    return;
}

Ticket *
queue_push (Queue *self, Ticket *guest)
{
    if (self->size > 0)
    {
        self->rear->next = guest;
        self->rear = guest;
        self->size++;
        return guest;
    }
    else
    {
        self->rear = guest;
        self->head = guest;
        self->size++;
        return guest;
    }
}

Ticket *
queue_sift (Queue *self)
{
    if (self->size > 1)
    {
        Ticket *ret = self->head;
        self->head = self->head->next;
        self->size--;
        return (ret);
    }
    else if (self->size == 1)
    {
        Ticket *ret = self->head;
        self->head = NULL;
        self->size--;
        return (self->head);
    }
    else
    {
        mkwarn ("Sifting a queue of size zero.");
        return NULL;
    }
}

void
queue_pop (Queue *self)
{
    if (self->size > 1)
    {
        Ticket *tmp = self->head->next;
        mk_free (self->head);
        self->head = tmp;
        self->size--;    
    }
    else if (self->size == 1)
    {
        mk_free (self->head);
        self->size--;
    }
    else
    {
        mkwarn ("Popping a queue of size zero.");
    }
}

void
test_one (void)
{
    Queue *queue = queue_lineup ();
    Ticket *test_tkt;
    queue_push (queue, ticket_new (7, 'c'));
    test_tkt = queue_push (queue, ticket_new (8, 'a'));
    mk_free (queue);
    printf ("%d %c",test_tkt->a, test_tkt->b);
    /* output should be `8 a`. This proves that freeing queue only frees the struct, not the content pointed to by pointers in that struct */
    return;
}

int main (void)
{
    test_one ();
}