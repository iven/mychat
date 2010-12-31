/*
 * =====================================================================================
 *
 *       Filename:  protocol.h
 *
 *    Description:  Interface of ChatSys protocol.
 *
 *        Version:  1.0
 *        Created:  2010年12月10日 19时50分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#ifndef  PROTOCOL_INC
#define  PROTOCOL_INC

#include    "sock_wrapper.h"
#include    "message.h"

enum chat_type {
    CHAT_SERVER,
    CHAT_CLIENT,
};              /* ----------  end of enum chat_type  ---------- */
typedef enum chat_type Chat_type;

int chat_protocol_init ( Chat_type type, int server_port, const char *server_name );
int chat_protocol_exit ( int my_fd );

int chat_msg_push ( Chat_msg *msg );            /* Push message to the sending queue */
Chat_msg *chat_msg_pop ( void );                /* Pop message from the receiving queue */

int chat_client_login ( int fd, const char *username ); /* Send login message to the server */
int chat_client_logout ( int fd );              /* Send logout message to the server */

int chat_server_main ( int server_fd );         /* Main loop used by the server */

#endif   /* ----- #ifndef PROTOCOL_INC  ----- */
