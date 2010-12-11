/*
 * =====================================================================================
 *
 *       Filename:  tcp_client.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010年11月19日 18时41分32秒
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
#include    <unistd.h>
#include	<pthread.h> 
#include	"sock_wrapper.h" 

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  recv_daemon
 *  Description:  receive messages from server.
 * =====================================================================================
 */
    static void
recv_daemon (void *fd)
{
    Chat_msg msg;
    int client_fd = (int) fd;
    while(1) {
        if (chat_recv(client_fd, &msg) < 0) {
            exit(1);
        }
        printf("\nClient says: %s\n", msg.text);
    }
}		/* -----  end of static function recv_daemon  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  send_daemon
 *  Description:  send messages to server.
 * =====================================================================================
 */
    static void
send_daemon (void *fd)
{
    Chat_msg msg;
    int client_fd = (int) fd;
    while (1) {
        printf("Say something: ");
        scanf("%s", msg.text);
        if (chat_send(client_fd, &msg) < 0) {
            exit(1);
        }
    }
    return ;
}		/* -----  end of static function send_daemon  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
    int
main (int argc, char *argv[])
{
    int client_fd;
    pthread_t tid[2];

    if (argc != 2) {
        printf("Usage: ./tcp_client hostname.\n");
        exit(1);
    }

    client_fd = chat_client_init(6666, argv[1]);
    if (client_fd < 0) {
        exit(1);
    }

    pthread_create(&tid[0], NULL, (void *) recv_daemon, (void *) client_fd);
    pthread_create(&tid[1], NULL, (void *) send_daemon, (void *) client_fd);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    if (chat_exit(client_fd) < 0) {
        exit(1);
    }
    return 0;
}		/* -----  end of function main  ----- */

