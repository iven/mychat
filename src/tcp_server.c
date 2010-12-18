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

static Queue *user_queue;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  send_to_user
 *  Description:  Send message to all users on the user queue.
 * =====================================================================================
 */
    static void
send_to_user ( Queue_node *node, void *msg )
{
    User_info *info = node->data;
    chat_send(info->fd, msg);
    return ;
}		/* -----  end of static function send_to_user  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  remove_user
 *  Description:  Remove user information from the user queue.
 * =====================================================================================
 */
    static void
remove_user ( Queue_node *node, void *fd )
{
    int client_fd = (int) fd;
    User_info *user_info = (User_info *) node->data;
    if (user_info->fd == client_fd) {
        printf("User %s logged out!\n", user_info->name);
        queue_node_destroy(node, (QUEUE_DESTROY) user_info_destroy);
        chat_exit(client_fd);
    }
    return ;
}		/* -----  end of static function remove_user  ----- */

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
    Queue_node *node;
    while (1) {
        msg = chat_pop_message();
        if (msg->type != CHAT_MSG_ACK) {
            sn++;
        }
        switch ( msg->type ) {
            case CHAT_MSG_LOGIN:
                printf("User %s logged in!\n", msg->text);
                node = queue_node_new(user_info_new(msg->fd, msg->text));
                queue_push(user_queue, node);
                break;
            case CHAT_MSG_LOGOUT:
                queue_foreach(user_queue, (QUEUE_CALLBACK) remove_user, (void *) msg->fd);
                break;
            case CHAT_MSG_CHAT:
                queue_foreach(user_queue, (QUEUE_CALLBACK) send_to_user, (void *) msg);
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
 *  Description:  
 * =====================================================================================
 */
    int
main (void)
{
    int server_fd, client_fd;
    pthread_t tid;
    user_queue = queue_new();
    server_fd = chat_protocol_init(CHAT_SERVER, 6666, NULL);
    if (server_fd < 0) {
        exit(1);
    }
    pthread_create(&tid, NULL, (void *) process_thread, NULL);
    printf("Waiting for client.\n");
    while (1) {
        client_fd = chat_server_accept_client(server_fd);
        if (client_fd < 0) {
            exit(1);
        }
        pthread_create(&tid, NULL, (void *) chat_recv_thread, (void *) client_fd);
    }
    if (chat_protocol_exit(server_fd) < 0) {
        exit(1);
    }
    return 0;
}		/* -----  end of function main  ----- */
