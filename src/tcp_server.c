/*
 * =====================================================================================
 *
 *       Filename:  tcp_server.c
 *
 *    Description:  
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
#include	<pthread.h> 
#include	"user_info.h" 
#include	"protocol.h" 
#include	"queue.h" 

static User_queue *user_queue;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  process_thread
 *  Description:  Process messages.
 * =====================================================================================
 */
    static int
process_thread ( void )
{
    static int sn = 0;
    Chat_msg *msg;
    User_info *user_info;
    char buf[MAX_TEXT_LEN + MAX_NAME_LEN + 2];
    char *names;
    while (1) {
        msg = chat_pop_message();               /* Wait for messages */
        if (msg->type != CHAT_MSG_ACK) {
            msg->sn = ++sn;
        }
        switch ( msg->type ) {
            case CHAT_MSG_LOGIN:                /* Add user to queue */
                printf("User %s logged in!\n", msg->text);
                user_info = user_info_new(msg->fd, msg->text);
                user_queue_add(user_queue, user_info);
                break;
            case CHAT_MSG_LOGOUT:               /* Remove user from queue */
                user_info = user_queue_get_from_fd(user_queue, msg->fd);
                printf("User %s logged out!\n", user_info->name);
                user_queue_remove(user_queue, user_info);
                break;
            case CHAT_MSG_CHAT:                 /* Prefix username and send to all */
                user_info = user_queue_get_from_fd(user_queue, msg->fd);
                sprintf(buf, "%s: %s", user_info->name, msg->text);
                buf[MAX_TEXT_LEN - 1] = '\0';
                strcpy(msg->text, buf);
                user_queue_send_to_all(user_queue, msg);
                break;
            case CHAT_MSG_LIST:                 /* Get usernames and send back */
                names = user_queue_get_names(user_queue);
                printf("User %s requested user list.\n", user_info->name);
                strcpy(msg->text, names);
                free(names);
                chat_send(msg->fd, msg);
                break;
            default:
                break;
        }				/* -----  end switch  ----- */
        chat_msg_destroy(msg);
    }
    return 0;
}		/* -----  end of static function process_thread  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Main function.
 * =====================================================================================
 */
    int
main (void)
{
    int server_fd, client_fd;
    pthread_t tid;

    user_queue = user_queue_new();              /* User information queue */
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
     *  Wait for clients and create receiving threads for them.
     *-----------------------------------------------------------------------------*/
    while (1) {
        client_fd = chat_server_accept_client(server_fd);
        if (client_fd < 0) {
            exit(1);
        }
        pthread_create(&tid, NULL, (void *) chat_recv_thread, (void *) client_fd);
    }
    /*-----------------------------------------------------------------------------
     *  ChatSys finalization.
     *-----------------------------------------------------------------------------*/
    if (chat_protocol_exit(server_fd) < 0) {
        exit(1);
    }
    return 0;
}		/* -----  end of function main  ----- */
