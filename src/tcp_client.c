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
#include	"protocol.h" 

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
    while (1) {
        msg = chat_pop_message();
        if (msg->type != CHAT_MSG_ACK) {
            sn++;
        }
        switch ( msg->type ) {
            case CHAT_MSG_CHAT:
                printf("%s\n", msg->text);
                break;
            case CHAT_MSG_LIST:
                printf("\033[0;32;40m[Online users: %s]\033[0m\n", msg->text);
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
 *         Name:  send_daemon
 *  Description:  Send user input messages to server.
 * =====================================================================================
 */
    static void
send_daemon ( int client_fd )
{
    Chat_msg *msg = chat_msg_new();
    char text[4096];
    int i;
    while (1) {
        i = -1;
        do {
            text[++i] = getchar();
        } while (text[i] != '\n' && text[i] != EOF);
        if (text[i] == '\n') {
            text[i] = '\0';
            if (strcmp(text, "/list") == 0) {
                msg->type = CHAT_MSG_LIST;
            } else {
                msg->type = CHAT_MSG_CHAT;
                strcpy(msg->text, text);
            }
            chat_send(client_fd, msg);
        } else if (text[i] == EOF) {
            break;
        }
    }
    msg->type = CHAT_MSG_LOGOUT;
    chat_send(client_fd, msg);
    chat_msg_destroy(msg);
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

    if (argc != 3) {
        printf("Usage: ./tcp_client hostname username.\n");
        exit(1);
    }

    client_fd = chat_protocol_init(CHAT_CLIENT, 6666, argv[1]);
    if (client_fd < 0) {
        exit(1);
    }

    /*-----------------------------------------------------------------------------
     *  Send login message to server.
     *-----------------------------------------------------------------------------*/
    Chat_msg *msg = chat_msg_new();
    msg->type = CHAT_MSG_LOGIN;
    strcpy(msg->text, argv[2]);
    chat_send(client_fd, msg);
    chat_msg_destroy(msg);

    pthread_create(&tid[0], NULL, (void *) chat_recv_thread, (void *) client_fd);
    pthread_create(&tid[1], NULL, (void *) process_thread, (void *) client_fd);

    send_daemon(client_fd);                              /* Loop here */

    if (chat_protocol_exit(client_fd) < 0) {
        exit(1);
    }
    return 0;
}		/* -----  end of function main  ----- */

