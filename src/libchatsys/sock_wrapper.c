/*
 * =====================================================================================
 *
 *       Filename:  sock_wrapper.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010年11月19日 20时41分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#include    <sys/types.h>
#include    <sys/socket.h>
#include    <netinet/in.h>
#include    <string.h>
#include    <errno.h>
#include    <stdio.h>
#include    <unistd.h>
#include    <arpa/inet.h>
#include	<netdb.h> 
#include	"sock_wrapper.h" 

#define LISTENQ 20                              /*  */
static struct sockaddr_in server_addr, client_addr;
static int server_fd, client_fd;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_server_init
 *  Description:  Initial chat server socket init.
 * =====================================================================================
 */
    int
chat_server_init (void)
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        return -1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    if (bind(server_fd, (struct sockaddr *) &server_addr,
                sizeof(struct sockaddr_in)) < 0) {
        return -2;
    }
    if (listen(server_fd, LISTENQ) == -1) {
        return -3;
    }
    return 0;
}		/* -----  end of function chat_server_init  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_server_exit
 *  Description:  Shutdown char server socket.
 * =====================================================================================
 */
    int
chat_server_exit (void)
{
    if (close(server_fd) < 0) {
        return -1;
    } else {
        return 0;
    }
}		/* -----  end of function chat_server_exit  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_server_accept_client
 *  Description:  Accept new client and interact wih it.
 * =====================================================================================
 */
    int
chat_server_accept_client (void)
{
    client_fd = accept(server_fd, NULL,
            (socklen_t *) sizeof(struct sockaddr_in));
    if (client_fd < 0) {
        return -1;
    }
    return client_fd;
}		/* -----  end of function chat_server_accept_client  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_server_close_client
 *  Description:  Close client connection.
 * =====================================================================================
 */
    int
chat_server_close_client (int fd)
{
    if (close(fd) < 0) {
        return -1;
    }
    return 0;
}		/* -----  end of function chat_server_close_client  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_server_send
 *  Description:  Send a message to client.
 * =====================================================================================
 */
    int
chat_server_send (int fd, const char *message, int size)
{
    if (send(fd, message, size, 0) < 0) {
        return -1;
    }
    return 0;
}		/* -----  end of function chat_server_send  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_server_recv
 *  Description:  Receive message from server.
 * =====================================================================================
 */
    int
chat_server_recv (int fd, char *buf, int size)
{
    return recv(fd, buf, MAX_MSG_SIZE, 0);
}		/* -----  end of function chat_server_recv  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_client_init
 *  Description:  Initial chat client socket init.
 * =====================================================================================
 */
    int
chat_client_init (const char *server_name)
{
    struct hostent *server_host;
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        return -1;
    }
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = htons(SERVER_PORT);
    if ((server_host = gethostbyname(server_name)) == NULL) {
        return -2;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr = *((struct in_addr *) server_host->h_addr);
    if (connect(client_fd, (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        return -3;
    }
    return 0;
}		/* -----  end of function chat_client_init  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_client_exit
 *  Description:  Shutdown client socket.
 * =====================================================================================
 */
    int
chat_client_exit (void)
{
    if (close(client_fd) < 0) {
        return -1;
    }
    return 0;
}		/* -----  end of function chat_client_exit  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_client_recv
 *  Description:  Receive message from server.
 * =====================================================================================
 */
    int
chat_client_recv (char *buf, int size)
{
    return recv(client_fd, buf, MAX_MSG_SIZE, 0);
}		/* -----  end of function chat_client_recv  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_client_send
 *  Description:  Send a message to server.
 * =====================================================================================
 */
    int
chat_client_send (const char *message, int size)
{
    if (send(client_fd, message, size, 0) < 0) {
        return -1;
    }
    return 0;
}		/* -----  end of function chat_client_send  ----- */

