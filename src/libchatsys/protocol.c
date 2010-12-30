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

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <pthread.h> 
#include    "protocol.h"
#include    "msg_queue.h"

static Chat_msg_queue *msg_queue_in;
static Chat_msg_queue *msg_queue_out;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_recv_thread
 *  Description:  Thread for receive messages.
 * =====================================================================================
 */
    static int
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
        chat_msg_queue_push(msg_queue_in, msg); /* Push message to the queue */
    }
}                                               /* -----  end of static function chat_recv_thread  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_send_thread
 *  Description:  Thread for send messages.
 * =====================================================================================
 */
    static void
chat_send_thread ( int fd )
{
    Chat_msg *msg;
    int retval;
    while (1) {
        msg = chat_msg_queue_pop(msg_queue_out);
        retval = chat_send(msg->fd, msg);
        chat_msg_destroy(msg);
    }
}       /* -----  end of static function chat_send_thread  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_server_main
 *  Description:  Wait for clients and create receiving threads for them.
 * =====================================================================================
 */
    int
chat_server_main ( int server_fd )
{
    int client_fd;
    pthread_t tid;
    while (1) {
        client_fd = chat_server_accept_client(server_fd);
        if (client_fd < 0) {
            exit(1);
        }
        pthread_create(&tid, NULL,
                (void *) chat_recv_thread, (void *) (unsigned long) client_fd);
    }
}                                               /* -----  end of static function chat_server_main  ----- */

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
    pthread_t tid;
    /*-----------------------------------------------------------------------------
     *  Create in and out queues.
     *-----------------------------------------------------------------------------*/
    msg_queue_in = chat_msg_queue_new();
    if (msg_queue_in == NULL) {                 /* init in message queue */
        return -1;
    }
    msg_queue_out = chat_msg_queue_new();
    if (msg_queue_out == NULL) {                /* init out message queue */
        return -2;
    }
    /*-----------------------------------------------------------------------------
     *  Initialize socket and create receiving and sending threads.
     *-----------------------------------------------------------------------------*/
    if (type == CHAT_SERVER) {                  /* server init */
        my_fd = chat_server_init(server_port);
    } else if (type == CHAT_CLIENT) {           /* client init */
        if (server_name == NULL) {
            return -3;
        }
        my_fd = chat_client_init(server_port, server_name);
        pthread_create(&tid, NULL,              /* Create receiving thread */
                (void *) chat_recv_thread, (void *) (unsigned long) my_fd);
    } else {                                    /* what's this? */
        return -4;
    }
    if (my_fd < 0) {                            /* init failed */
        return -5;
    }
    pthread_create(&tid, NULL,                  /* Create sending thread */
            (void *) chat_send_thread, (void *) (unsigned long) my_fd);
    return my_fd;
}                                               /* -----  end of function chat_protocol_init  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_protocol_exit
 *  Description:  Finalize ChatSys protocol.
 * =====================================================================================
 */
    int
chat_protocol_exit ( int my_fd )
{
    chat_msg_queue_destroy(msg_queue_in);       /* destroy in message queue */
    chat_msg_queue_destroy(msg_queue_out);      /* destroy out message queue */
    chat_exit(my_fd);                           /* exit server or client */

    return 0;
}                                               /* -----  end of function chat_protocol_exit  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_client_login
 *  Description:  Send login message to server.
 * =====================================================================================
 */
    int
chat_client_login ( int fd, const char *username )
{
    Chat_msg *msg = chat_msg_new();
    msg->type = CHAT_MSG_LOGIN;
    msg->fd = fd;
    strcpy(msg->text, username);
    chat_msg_push(msg);
    chat_msg_destroy(msg);
    return 0;
}                                               /* -----  end of function chat_client_login  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_client_logout
 *  Description:  Send logout message to server.
 * =====================================================================================
 */
    int
chat_client_logout ( int fd )
{
    Chat_msg *msg = chat_msg_new();
    msg->type = CHAT_MSG_LOGOUT;
    msg->fd = fd;
    chat_msg_push(msg);
    chat_msg_destroy(msg);
    return 0;
}                                               /* -----  end of function chat_client_logout  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_msg_push
 *  Description:  Push message to msg_queue_out.
 * =====================================================================================
 */
    int
chat_msg_push ( Chat_msg *origin_msg )
{
    /*-----------------------------------------------------------------------------
     *  Give it a copy before pushing, because members of origin_msg may be
     *  changed after pushing.
     *-----------------------------------------------------------------------------*/
    Chat_msg *msg = chat_msg_new_from_msg(origin_msg);
    chat_msg_queue_push(msg_queue_out, msg);    /* Just push it to the out queue */
    return 0;
}       /* -----  end of function chat_msg_push  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_msg_pop
 *  Description:  Pop message from msg_queue_in.
 * =====================================================================================
 */
    Chat_msg *
chat_msg_pop ( void )
{
    return chat_msg_queue_pop(msg_queue_in);    /* Just pop from the in queue */
}       /* -----  end of function chat_msg_pop  ----- */

