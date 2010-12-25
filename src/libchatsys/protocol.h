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

int chat_recv_thread ( int fd );

Chat_msg * chat_pop_message ( void );
int chat_push_message ( Chat_msg *msg );

int chat_client_login ( int fd, const char *username );
int chat_client_logout ( int fd );

#endif   /* ----- #ifndef PROTOCOL_INC  ----- */
