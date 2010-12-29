/*
 * =====================================================================================
 *
 *       Filename:  msg_queue.c
 *
 *    Description:  Implemention of message queue.
 *
 *        Version:  1.0
 *        Created:  2010年12月29日 17时31分52秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    "message.h"
#include    "msg_queue.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_msg_queue_new
 *  Description:  Create a new msg_queue.
 * =====================================================================================
 */
    Msg_queue *
chat_msg_queue_new ( void )
{
    Msg_queue *msg_queue;
    Queue *queue;
    Event *event;

    msg_queue = (Msg_queue *) calloc(1, sizeof(Msg_queue));
    if (msg_queue == NULL) {
        goto fail_msg_queue;
    }
    if ((queue = queue_new()) == NULL) {        /* init in message queue */
        goto fail_queue;
    }
    if ((event = event_new(0)) == NULL) {       /* init new message event */
        goto fail_event;
    }
    msg_queue->queue = queue;
    msg_queue->event = event;
    return msg_queue;

fail_event:
    free(queue);
fail_queue:
    free(msg_queue);
fail_msg_queue:
    return NULL;
}       /* -----  end of function chat_msg_queue_new  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_msg_queue_destroy
 *  Description:  Destroy a msg_queue.
 * =====================================================================================
 */
    int
chat_msg_queue_destroy ( Msg_queue *msg_queue )
{
    if (msg_queue == NULL ||
            msg_queue->queue == NULL ||
            msg_queue->event == NULL) {
        return -1;
    }
    event_destroy(msg_queue->event);
    queue_destroy(msg_queue->queue, (QUEUE_DESTROY)chat_msg_destroy);
    free(msg_queue);
    return 0;
}       /* -----  end of function chat_msg_queue_destroy  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_msg_queue_pop
 *  Description:  Pop new message from the @msg_queue.
 * =====================================================================================
 */
    Chat_msg *
chat_msg_queue_pop ( Msg_queue *msg_queue )
{
    Queue_node *node;
    Chat_msg *msg;

    event_wait(msg_queue->event);               /* Wait for new message event */
    node = queue_pop(msg_queue->queue);                /* Pop the node */
    if (node == NULL) {
        return NULL;
    }
    msg = node->data;
    queue_node_destroy(node, NULL);
    return msg;
}                                               /* -----  end of function chat_msg_queue_pop  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_msg_queue_push
 *  Description:  Push new message to the @msg_queue.
 * =====================================================================================
 */
    int
chat_msg_queue_push ( Msg_queue *msg_queue, Chat_msg *msg )
{
    Queue_node *node;

    node = queue_node_new(msg);                 /* Create a new node */
    if (node == NULL) {
        chat_msg_destroy(msg);
        return -1;
    }
    queue_push(msg_queue->queue, node);                /* Push the node */
    event_post(msg_queue->event);               /* Post a new message event */
    return 0;
}                                               /* -----  end of function chat_msg_queue_push  ----- */

