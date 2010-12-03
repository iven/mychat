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
    int new_fd = * (int *) fd;
    int numbytes;
    char message[MAX_MSG_SIZE];
    while(1) {
        numbytes = chat_server_recv(new_fd, message, MAX_MSG_SIZE);
        if (numbytes < 0) {
            perror("recv");
            exit(1);
        }
        message[numbytes] = '\0';
        printf("\nClient says: %s\n", message);
    }
    chat_server_close_client(new_fd);
    return ;
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
    int new_fd = * (int *) fd;
    char message[MAX_MSG_SIZE];
    while (1) {
        printf("Say something: ");
        scanf("%s", message);
        if (chat_server_send(new_fd, message, strlen(message)) < 0) {
            perror("send");
            exit(1);
        }
    }
    chat_server_close_client(new_fd);
    return ;
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
    if (chat_server_init()) {
        perror("init");
        exit(1);
    }
    printf("Waiting for client.\n");
    while (1) {
        pthread_t tid;
        int new_fd = chat_server_accept_client();
        if (new_fd < 0) {
            perror("accept");
            exit(1);
        }
        printf("New client!\n");
        pthread_create(&tid, NULL, (void *) recv_daemon, (void *) &new_fd);
        pthread_create(&tid, NULL, (void *) send_daemon, (void *) &new_fd);
    }
    if (chat_server_exit()) {
        perror("exit");
        exit(1);
    }
    return 0;
}		/* -----  end of function main  ----- */
