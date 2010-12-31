/*
 * =====================================================================================
 *
 *       Filename:  sock_wrapper.h
 *
 *    Description:  Interface of socket wrapper.
 *
 *        Version:  1.0
 *        Created:  2010年11月19日 20时54分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#ifndef  SOCK_WRAPPER_INC
#define  SOCK_WRAPPER_INC

#include    "message.h" 

#define QUEUE_SIZE 20                              /* Listen queue size */

int chat_server_init (int server_port);
int chat_client_init (int server_port, const char *server_name);

int chat_server_accept_client (int server_fd);

int chat_send (int client_fd, const Chat_msg *msg);
int chat_recv (int client_fd, Chat_msg *buf);
int chat_exit (int fd);

#endif   /* ----- #ifndef SOCK_WRAPPER_INC  ----- */
