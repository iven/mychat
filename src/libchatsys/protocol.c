/*
 * =====================================================================================
 *
 *       Filename:  protocol.c
 *
 *    Description:  Implemention of ChatSys protocol.
 *
 *        Version:  1.0
 *        Created:  2010年12月10日 19时54分30秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#include	<stdio.h> 
#include	<stdlib.h> 
#include	"protocol.h"
#include	"queue.h"

static Queue *msg_queue;
static Event *msg_event;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_protocol_init
 *  Description:  Initialize ChatSys protocol.
 * =====================================================================================
 */
    int
chat_protocol_init ( Chat_type type, int server_port, const char *server_name )
{
    int my_fd;
    if (type == CHAT_SERVER) {                  /* server init */
        my_fd = chat_server_init(server_port);
    } else if (type == CHAT_CLIENT) {           /* client init */
        if (server_name == NULL) {
            return -1;
        }
        my_fd = chat_client_init(server_port, server_name);
    } else {                                    /* what's this? */
        return -2;
    }
    if (my_fd < 0) {                            /* init failed */
        return -3;
    }

    if ((msg_queue = queue_new()) == NULL) {    /* init message queue */
        return -4;
    }
    if ((msg_event = event_new(0)) == NULL) { /* init new message event */
        return -5;
    }
    return my_fd;
}		/* -----  end of function chat_protocol_init  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_protocol_exit
 *  Description:  Finalize ChatSys protocol.
 * =====================================================================================
 */
    int
chat_protocol_exit ( int my_fd )
{
    event_destroy(msg_event);                   /* destroy new message event */
    queue_destroy(msg_queue,                    /* destroy message queue */
            (QUEUE_DESTROY) chat_msg_destroy);
    chat_exit(my_fd);                           /* exit server or client */

    return 0;
}		/* -----  end of function chat_protocol_exit  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_recv_thread
 *  Description:  Thread for receive messages.
 * =====================================================================================
 */
    int
chat_recv_thread ( int fd )
{
    Chat_msg *msg;
    int retval;
    while (1) {
        msg = chat_msg_new();                   /* Create a new message */
        if (msg == NULL) {
            return -1;
        }
        retval = chat_recv(fd, msg);
        if (retval < 0) {                       /* Receive message from @fd */
            return -2;
        } else if (retval == 0) {               /* client quit normally */
            return 0;
        }
        msg->fd = fd;
        chat_push_message(msg);                 /* Push message to the queue */
    }
}		/* -----  end of static function chat_recv_thread  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_pop_message
 *  Description:  Pop new message from the queue.
 * =====================================================================================
 */
    Chat_msg *
chat_pop_message ( void )
{
    Queue_node *node;
    Chat_msg *msg;

    event_wait(msg_event);                      /* Wait for new message event */
    node = queue_pop(msg_queue);                /* Pop the node */
    if (node == NULL) {
        return NULL;
    }
    msg = node->data;
    queue_node_destroy(node, NULL);
    return msg;
}		/* -----  end of function chat_pop_message  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_push_message
 *  Description:  Push new message to the queue.
 * =====================================================================================
 */
    int
chat_push_message ( Chat_msg *msg )
{
    Queue_node *node;

    node = queue_node_new(msg);                 /* Create a new node */
    if (node == NULL) {
        chat_msg_destroy(msg);
        return -1;
    }
    queue_push(msg_queue, node);                /* Push the node */
    event_post(msg_event);                      /* Post a new message event */
    return 0;
}		/* -----  end of function chat_push_message  ----- */

