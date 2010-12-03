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
#include	"sock_wrapper.h" 

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  recv_daemon
 *  Description:  receive messages from client.
 * =====================================================================================
 */
    static void
recv_daemon (void)
{
    int numbytes;
    char message[MAX_MSG_SIZE];
    while(1) {
        numbytes = chat_server_recv(message, MAX_MSG_SIZE);
        if (numbytes < 0) {
            perror("recv");
            exit(1);
        }
        printf("\nClient says: %s\n", message);
    }
    return ;
}		/* -----  end of static function recv_daemon  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  send_daemon
 *  Description:  send messages to client.
 * =====================================================================================
 */
    static void
send_daemon (void)
{
    char message[MAX_MSG_SIZE];
    while (1) {
        printf("Say something: ");
        scanf("%s", message);
        if (chat_server_send(message, strlen(message)) < 0) {
            perror("send");
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
main (void)
{
    if (chat_server_init()) {
        perror("init");
        exit(1);
    }
    printf("Waiting for client.\n");
    chat_server_accept_client();
    if (fork() == 0) {
        recv_daemon();
        chat_server_close_client();
    } else {
        printf("New client!\n");
        send_daemon();
        chat_server_close_client();
    }
    while(waitpid(-1,NULL,WNOHANG) > 0) {
        ;
    }
    if (chat_server_exit()) {
        perror("exit");
        exit(1);
    }
    return 0;
}		/* -----  end of function main  ----- */
