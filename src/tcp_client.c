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
recv_daemon (void)
{
    int numbytes;
    char message[MAX_MSG_SIZE];
    while(1) {
        numbytes = chat_client_recv(message, MAX_MSG_SIZE);
        if (numbytes < 0) {
            perror("recv");
            exit(1);
        }
        message[numbytes] = '\0';
        printf("\nServer says: %s\n", message);
    }
    return ;
}		/* -----  end of static function recv_daemon  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  send_daemon
 *  Description:  send messages to server.
 * =====================================================================================
 */
    static void
send_daemon (void)
{
    char message[MAX_MSG_SIZE];
    while (1) {
        printf("Say something: ");
        scanf("%s", message);
        if (chat_client_send(message, strlen(message)) < 0) {
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
main (int argc, char *argv[])
{
    pthread_t tid[2];
    if (argc != 2) {
        printf("Usage: ./tcp_client hostname.\n");
        exit(1);
    }

    if (chat_client_init(argv[1])) {
        perror("init");
        exit(1);
    }

    pthread_create(&tid[0], NULL, (void *) recv_daemon, NULL);
    pthread_create(&tid[1], NULL, (void *) send_daemon, NULL);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    if (chat_client_exit()) {
        perror("exit");
        exit(1);
    }
    return 0;
}		/* -----  end of function main  ----- */

