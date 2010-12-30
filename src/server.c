/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description:  Implemention of chat server.
 *
 *        Version:  1.0
 *        Created:  2010年11月19日 18时37分05秒
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
#include    <errno.h>
#include    <string.h>
#include    <sys/wait.h>
#include    <unistd.h>
#include    <pthread.h> 
#include    "user_queue.h"
#include    "protocol.h"
#include    "queue.h"

static Chat_user_queue *user_queue;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  process_thread
 *  Description:  Process messages.
 * =====================================================================================
 */
    static int
process_thread ( void )
{
    Chat_msg *msg;
    Chat_user *user;
    char buf[MAX_TEXT_LEN + MAX_NAME_LEN + 2];
    char *names;
    while (1) {
        msg = chat_msg_pop();                   /* Wait for messages */
        switch ( msg->type ) {
            case CHAT_MSG_LOGIN:                /* Add user to queue */
                printf("User %s logged in!\n", msg->text);
                user = chat_user_new(msg->fd, msg->text);
                chat_user_queue_add(user_queue, user);
                break;
            case CHAT_MSG_LOGOUT:               /* Remove user from queue */
                user = chat_user_queue_get_from_fd(user_queue, msg->fd);
                printf("User %s logged out!\n", user->name);
                chat_user_queue_remove(user_queue, user);
                break;
            case CHAT_MSG_CHAT:                 /* Prefix username and send to all */
                user = chat_user_queue_get_from_fd(user_queue, msg->fd);
                sprintf(buf, "%s: %s", user->name, msg->text);
                buf[MAX_TEXT_LEN - 1] = '\0';
                strcpy(msg->text, buf);
                chat_user_queue_send_to_all(user_queue, msg);
                break;
            case CHAT_MSG_LIST:                 /* Get usernames and send back */
                names = chat_user_queue_get_names(user_queue);
                printf("User %s requested user list.\n", user->name);
                strcpy(msg->text, names);
                free(names);
                chat_msg_push(msg);
                break;
            default:
                break;
        }                                       /* -----  end switch  ----- */
        chat_msg_destroy(msg);
    }
    return 0;
}                                               /* -----  end of static function process_thread  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Main function.
 * =====================================================================================
 */
    int
main (void)
{
    int server_fd;
    pthread_t tid;

    user_queue = chat_user_queue_new();         /* User information queue */
    /*-----------------------------------------------------------------------------
     *  ChatSys initialization.
     *-----------------------------------------------------------------------------*/
    server_fd = chat_protocol_init(CHAT_SERVER, 6666, NULL);
    if (server_fd < 0) {
        exit(1);
    }
    printf("Chat server started, waiting for client...\n");
    /*-----------------------------------------------------------------------------
     *  Create a thread for managing messages.
     *-----------------------------------------------------------------------------*/
    pthread_create(&tid, NULL, (void *) process_thread, NULL);
    /*-----------------------------------------------------------------------------
     *  Accept clients and receive messages
     *-----------------------------------------------------------------------------*/
    chat_server_main(server_fd);
    /*-----------------------------------------------------------------------------
     *  ChatSys finalization.
     *-----------------------------------------------------------------------------*/
    if (chat_protocol_exit(server_fd) < 0) {
        exit(1);
    }
    return 0;
}                                               /* -----  end of function main  ----- */
