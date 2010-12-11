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
#include	"sock_wrapper.h" 

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  recv_daemon
 *  Description:  receive messages from client.
 * =====================================================================================
 */
    static void
recv_daemon (void *fd)
{
    int client_fd = (int) fd;
    Chat_msg msg;
    while(1) {
        if (chat_recv(client_fd, &msg) < 0) {
            exit(1);
        }
        printf("\nClient says: %s\n", msg.text);
    }
    chat_exit(client_fd);
}		/* -----  end of static function recv_daemon  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  send_daemon
 *  Description:  send messages to client.
 * =====================================================================================
 */
    static void
send_daemon (void *fd)
{
    int client_fd = (int) fd;
    Chat_msg msg;
    while (1) {
        printf("Say something: ");
        scanf("%s", msg.text);
        if (chat_send(client_fd, &msg) < 0) {
            exit(1);
        }
    }
    chat_exit(client_fd);
}		/* -----  end of static function send_daemon  ----- */

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
    server_fd = chat_server_init(6666);
    if (server_fd < 0) {
        exit(1);
    }
    printf("Waiting for client.\n");
    while (1) {
        client_fd = chat_server_accept_client(server_fd);
        if (client_fd < 0) {
            exit(1);
        }
        printf("New client!\n");
        pthread_create(&tid, NULL, (void *) recv_daemon, (void *) client_fd);
        pthread_create(&tid, NULL, (void *) send_daemon, (void *) client_fd);
    }
    if (chat_exit(server_fd) < 0) {
        exit(1);
    }
    return 0;
}		/* -----  end of function main  ----- */
