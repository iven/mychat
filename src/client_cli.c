/*
 * =====================================================================================
 *
 *       Filename:  client_cli.c
 *
 *    Description:  Implementation of chat client.
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
#include    <pthread.h>
#include    "protocol.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  display_prompt
 *  Description:  Display prompt for user input.
 * =====================================================================================
 */
    static void
display_prompt ( void )
{
    printf("\033[0;33;49m>> \033[0m");
    fflush(stdout);
    return ;
}       /* -----  end of static function display_prompt  ----- */

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
        msg = chat_msg_pop();
        printf("\r");                           /* Go to start of line */
        switch ( msg->type ) {
            case CHAT_MSG_CHAT:                 /* Chat message */
                printf("%s\n", msg->text);
                break;
            case CHAT_MSG_LIST:                 /* List users */
                printf("\033[0;32;49m[Online users: %s]\033[0m\n", msg->text);
                break;
        }               /* -----  end switch  ----- */
        chat_msg_destroy(msg);
        display_prompt();
    }
    return 0;
}       /* -----  end of static function process_thread  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  send_daemon
 *  Description:  Send user input messages to server.
 * =====================================================================================
 */
    static void
send_daemon ( int client_fd )
{
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
            msg->fd = client_fd;
            chat_msg_push(msg);
        } else if (text[i] == EOF) {            /* Exit */
            break;
        }
    }
    chat_msg_destroy(msg);
    return ;
}       /* -----  end of static function send_daemon  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  main function
 * =====================================================================================
 */
    int
main (int argc, char *argv[])
{
    int client_fd;
    pthread_t tid;

    if (argc != 3) {
        printf("Usage: chat_client_cli hostname username.\n");
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
    printf("\033[0;32;49mWelcome to ChatSys 0.1!\n  Type '/list' for online users.\n  Press 'Ctrl+D' for quiting.\033[0m\n");
    display_prompt();
    /*-----------------------------------------------------------------------------
     *  Send login message to server.
     *-----------------------------------------------------------------------------*/
    chat_client_login(client_fd, argv[2]);
    /*-----------------------------------------------------------------------------
     *  Create processing thread.
     *-----------------------------------------------------------------------------*/
    pthread_create(&tid, NULL, (void *) process_thread,
            (void *) (unsigned long) client_fd);
    send_daemon(client_fd);                              /* Loop here */
    /*-----------------------------------------------------------------------------
     *  When out of loop, send login message to server.
     *-----------------------------------------------------------------------------*/
    chat_client_logout(client_fd);
    /*-----------------------------------------------------------------------------
     *  ChatSys finalization.
     *-----------------------------------------------------------------------------*/
    if (chat_protocol_exit(client_fd) < 0) {
        exit(1);
    }
    printf("Bye!\n");

    return 0;
}       /* -----  end of function main  ----- */

