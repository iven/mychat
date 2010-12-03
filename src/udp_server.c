/*
 * =====================================================================================
 *
 *       Filename:  udp_server.c
 *
 *    Description:  UDP Server
 *
 *        Version:  1.0
 *        Created:  2010年11月13日 14时21分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#include	<stdio.h> 
#include	<stdlib.h> 
#include	<string.h> 
#include	<errno.h> 
#include	<unistd.h> 
#include	<sys/types.h> 
#include	<netinet/in.h> 
#include	<sys/socket.h>
#include	<sys/wait.h> 
#include    <arpa/inet.h>

#define	MYPORT 3490			/*  */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  main function
 * =====================================================================================
 */
	int
main (int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size, retval;
    char buf[128];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero), 8);

    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    while (1) {
        retval = recvfrom(sockfd, buf, 128, 0,
            (struct sockaddr *) &their_addr, &sin_size);
        printf("Received datagram from %s\n", inet_ntoa(their_addr.sin_addr));
        if (retval == 0) {
            perror("recvfrom");
            close(sockfd);
            break;
        }
        retval = sendto(sockfd, buf, 128, 0,
            (struct sockaddr *) &their_addr, sin_size);
    }

	return 0;
}		/* ----------  end of function main  ---------- */

