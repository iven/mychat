/*
 * =====================================================================================
 *
 *       Filename:  queue.h
 *
 *    Description:  Interface of queue.
 *
 *        Version:  1.0
 *        Created:  2010年12月04日 09时24分40秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#ifndef  QUEUE_INC
#define  QUEUE_INC

#include    "event.h"

struct queue_node {
    struct queue_node *next;
    void       *data;
};              /* ----------  end of struct queue_node  ---------- */
typedef struct queue_node Queue_node;

struct queue {
    Queue_node *head;
    Queue_node *tail;
    Event      *lock;
    int n_nodes;
};              /* ----------  end of struct queue  ---------- */
typedef struct queue Queue;

typedef void (*QUEUE_DESTROY)(void *);

Queue *queue_new(void);
int queue_destroy(Queue *queue, QUEUE_DESTROY func);

int queue_empty(Queue *queue);
#define queue_foreach(queue, node) \
    for (node = queue->tail; node != NULL; node = node->next)

Queue_node *queue_pop(Queue *queue);
int queue_push(Queue *queue, Queue_node *node);

int queue_lock (Queue *queue);
int queue_unlock (Queue *queue);

Queue_node *queue_node_new(void *data);
int queue_node_destroy(Queue_node *node, QUEUE_DESTROY func);

#endif   /* ----- #ifndef QUEUE_INC  ----- */
