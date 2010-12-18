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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_server_init
 *  Description:  Initial chat server socket init.
 * =====================================================================================
 */
    int
chat_server_init (int server_port)
{
    int server_fd;
    struct sockaddr_in addr;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("chat_server_init, socket");
        return -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(server_port);
    if (bind(server_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) {
        perror("chat_server_init, bind");
        return -2;
    }
    if (listen(server_fd, QUEUE_SIZE) == -1) {
        perror("chat_server_init, listen");
        return -3;
    }
    return server_fd;
}		/* -----  end of function chat_server_init  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_server_accept_client
 *  Description:  Accept new client and interact wih it.
 * =====================================================================================
 */
    int
chat_server_accept_client (int server_fd)
{
    int client_fd = accept(server_fd, NULL,
            (socklen_t *) sizeof(struct sockaddr_in));
    if (client_fd < 0) {
        perror("chat_server_accept_client, accept");
        return -1;
    }
    return client_fd;
}		/* -----  end of function chat_server_accept_client  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_client_init
 *  Description:  Initial chat client socket init.
 * =====================================================================================
 */
    int
chat_client_init (int server_port, const char *server_name)
{
    int client_fd;
    struct sockaddr_in server_addr;
    struct hostent *server_host;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("chat_client_init, socket");
        return -1;
    }
    if ((server_host = gethostbyname(server_name)) == NULL) {
        perror("chat_client_init, gethostbyname");
        return -2;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr = *((struct in_addr *) server_host->h_addr);
    if (connect(client_fd, (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        perror("chat_client_init, connect");
        return -3;
    }
    return client_fd;
}		/* -----  end of function chat_client_init  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_recv
 *  Description:  Receive message from @client_fd.
 * =====================================================================================
 */
    int
chat_recv (int client_fd, Chat_msg *buf)
{
    Chat_pdu pdu;
    int retval;
    retval = recv(client_fd, &pdu, sizeof(Chat_pdu), 0);
    if (retval < 0) {
        perror("chat_recv, recv");
        return retval;
    }
    chat_msg_parse(&pdu, buf);
    return retval;
}		/* -----  end of function chat_recv  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_send
 *  Description:  Send a message to @client_fd.
 * =====================================================================================
 */
    int
chat_send (int client_fd, const Chat_msg *msg)
{
    Chat_pdu pdu;
    chat_msg_format(&pdu, msg);
    if (send(client_fd, &pdu, sizeof(Chat_pdu), 0) < 0) {
        perror("chat_send, send");
        return -1;
    }
    return 0;
}		/* -----  end of function chat_send  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_exit
 *  Description:  Shutdown server or client.
 * =====================================================================================
 */
    int
chat_exit (int fd)
{
    if (close(fd) < 0) {
        perror("chat_exit, close");
        return -1;
    }
    return 0;
}		/* -----  end of function chat_exit  ----- */

