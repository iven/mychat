/*
 * =====================================================================================
 *
 *       Filename:  queue.c
 *
 *    Description:  Implemention of queue.
 *
 *        Version:  1.0
 *        Created:  2010年12月04日 09时34分40秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#include	<stdlib.h> 
#include	"queue.h" 

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  queue_new
 *  Description:  Create a new queue.
 * =====================================================================================
 */
    Queue *
queue_new ( void )
{
    Queue *queue = (Queue *) calloc(1, sizeof(Queue));
    if ((queue == NULL) || ((queue->lock = event_new(1)) == NULL)) {
        perror("queue_new");
        return NULL;
    }
    return queue;
}		/* -----  end of function queue_new  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  queue_destory
 *  Description:  Destroy a queue.
 * =====================================================================================
 */
    int
queue_destory ( Queue *queue )
{
    if (queue == NULL) {
        return -1;
    }
    queue_lock(queue);
    while (!queue_empty(queue)) {                     /* Destroy every node in the queue */
        queue_node_destroy(queue_pop(queue));
    }
    queue_unlock(queue);
    event_destroy(queue->lock);
    free(queue);
    return 0;
}		/* -----  end of function queue_destory  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  queue_empty
 *  Description:  
 * =====================================================================================
 */
    inline int
queue_empty ( Queue *queue )
{
    return (queue->tail == NULL);
}		/* -----  end of function queue_empty  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  queue_push
 *  Description:  Push a node to the queue.
 * =====================================================================================
 */
    int
queue_push ( Queue *queue, Queue_node *node )
{
    if (queue == NULL || node == NULL) {
        return -1;
    }
    queue_lock(queue);
    if (queue_empty(queue)) {                  /* Empty queue, init tail pointer */
        queue->tail = node;
    } else {                                    /* Update next pointer of queue head */
        queue->head->next = node;
    }
    queue->head = node;                         /* Update queue head */
    queue->n_nodes++;                           /* Update queue nodes count */
    node->next = NULL;                          /* Next node must be NULL */
    queue_unlock(queue);
    return 0;
}		/* -----  end of function queue_push  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  queue_pop
 *  Description:  Pop a node from the queue.
 * =====================================================================================
 */
    Queue_node *
queue_pop ( Queue *queue )
{
    if (queue == NULL || queue_empty(queue)) {
        return NULL;
    }
    queue_lock(queue);
    Queue_node *node = queue->tail;
    queue->tail = node->next;
    queue->n_nodes--;
    queue_unlock(queue);
    return node;
}		/* -----  end of function queue_pop  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  queue_lock
 *  Description:  Lock a queue.
 * =====================================================================================
 */
    int
queue_lock ( Queue *queue )
{
    return event_wait(queue->lock);
}		/* -----  end of function queue_lock  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  queue_unlock
 *  Description:  Unlock a queue.
 * =====================================================================================
 */
    int
queue_unlock ( Queue *queue )
{
    return event_post(queue->lock);
}		/* -----  end of function queue_unlock  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  queue_node_new
 *  Description:  Create a new queue node.
 * =====================================================================================
 */
    Queue_node *
queue_node_new ( void *data )
{
    Queue_node *node;
    node =  (Queue_node *) calloc(1, sizeof(Queue_node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    return node;
}		/* -----  end of function queue_node_new  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  queue_node_destroy
 *  Description:  Destroy a queue node.
 *                You _must_ free the data yourself.
 * =====================================================================================
 */
    int
queue_node_destroy ( Queue_node *node )
{
    free(node);
    return 0;
}		/* -----  end of function queue_node_destroy  ----- */
