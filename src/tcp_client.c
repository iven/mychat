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
 *         Name:  display_prompt
 *  Description:  Display prompt for user input.
 * =====================================================================================
 */
    static void
display_prompt ( void )
{
    printf(">> ");
    fflush(stdout);
    return ;
}		/* -----  end of static function display_prompt  ----- */

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
    while (1) {
        msg = chat_pop_message();
        printf("\r");                           /* Go to start of line */
        switch ( msg->type ) {
            case CHAT_MSG_CHAT:                 /* Chat message */
                printf("%s\n", msg->text);
                break;
            case CHAT_MSG_LIST:                 /* List users */
                printf("\033[0;32;40m[Online users: %s]\033[0m\n", msg->text);
                break;
        }				/* -----  end switch  ----- */
        chat_msg_destroy(msg);
        display_prompt();
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
    static int sn = 0;
    int i;
    Chat_msg *msg = chat_msg_new();
    char text[MAX_TEXT_LEN];
    while (1) {
        i = -1;
        do {                                    /* Get a message from shell */
            text[++i] = getchar();
        } while (text[i] != '\n' && text[i] != EOF);
        if (text[i] == '\n') {                  /* New message */
            text[i] = '\0';
            if (strcmp(text, "/list") == 0) {   /* List users */
                msg->type = CHAT_MSG_LIST;
            } else {                            /* Chat message */
                msg->type = CHAT_MSG_CHAT;
                strcpy(msg->text, text);
            }
            msg->sn = ++sn;
            chat_send(client_fd, msg);
        } else if (text[i] == EOF) {            /* Exit */
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
    /*-----------------------------------------------------------------------------
     *  ChatSys initialization.
     *-----------------------------------------------------------------------------*/
    client_fd = chat_protocol_init(CHAT_CLIENT, 6666, argv[1]);
    if (client_fd < 0) {
        exit(1);
    }
    /*-----------------------------------------------------------------------------
     *  Welcome message.
     *-----------------------------------------------------------------------------*/
    printf("Welcome to ChatSys 0.1!\n  Type '/list' for online users.\n  Press 'Ctrl+D' for quiting.\n");
    display_prompt();
    /*-----------------------------------------------------------------------------
     *  Send login message to server.
     *-----------------------------------------------------------------------------*/
    Chat_msg *msg = chat_msg_new();
    msg->type = CHAT_MSG_LOGIN;
    strcpy(msg->text, argv[2]);
    chat_send(client_fd, msg);
    chat_msg_destroy(msg);
    /*-----------------------------------------------------------------------------
     *  Start threads.
     *-----------------------------------------------------------------------------*/
    pthread_create(&tid[0], NULL, (void *) chat_recv_thread, (void *) client_fd);
    pthread_create(&tid[1], NULL, (void *) process_thread, (void *) client_fd);
    send_daemon(client_fd);                              /* Loop here */
    /*-----------------------------------------------------------------------------
     *  ChatSys finalization.
     *-----------------------------------------------------------------------------*/
    if (chat_protocol_exit(client_fd) < 0) {
        exit(1);
    }
    printf("Bye!\n");

    return 0;
}		/* -----  end of function main  ----- */

