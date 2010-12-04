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

struct queue {
    Queue_node *head;
    Queue_node *tail;
    int n_nodes;
};				/* ----------  end of struct queue  ---------- */
typedef struct queue Queue;

struct queue_node {
    Queue_node *next;
    void       *data;
};				/* ----------  end of struct queue_node  ---------- */
typedef struct queue_node Queue_node;

Queue *queue_new(void);
int queue_destroy(Queue *queue);
int queue_push(Queue *queue, Queue_node *node);
Queue_node *queue_pop(Queue *queue);

#endif   /* ----- #ifndef QUEUE_INC  ----- */
