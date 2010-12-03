/*
 * =====================================================================================
 *
 *       Filename:  udp_client.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010年11月13日 14时44分05秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 3490                               /* Server的端口 */
#define MAXDATASIZE 100                         /*一次可以读的最大字节数 */

int main(int argc, char *argv[])

{
    int sockfd, numbytes, sin_size;
    char buf[MAXDATASIZE] = "Hello, world!\n";
    struct hostent *he;                         /* 主机信息 */
    struct sockaddr_in their_addr;              /* 对方地址信息 */

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    /* get the host info */
    if ((he=gethostbyname(argv[1])) == NULL) {
        herror("gethostbyname");
        exit(1);
    }

    if ((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1) {
        perror("socket");
        exit(1);
    }

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);          /* short, NBO */
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(their_addr.sin_zero), 8);           /* 其余部分设成0 */

    while (1) {
        printf("Please input data: ");
        fgets(buf, MAXDATASIZE, stdin);
        if (memcmp(buf, "file", 4) == 0) {      /* upload file */
            char filename[MAXDATASIZE];
            char *p = filename, *q = buf + 5;
            while (*q != '\n') {
                *p++ = *q++;
            }
            FILE *fp = fopen(filename, "r");
            if (fp == NULL) {
                printf("File opening error.\n");
            } else {
                do {
                    numbytes = sendto(sockfd, buf, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, sizeof(their_addr));
                    if (numbytes == -1) {
                        perror("sendto");
                        exit(1);
                    }
                    numbytes = fread(buf, MAXDATASIZE, 1, fp);
                } while(numbytes != 0);
                fclose(fp);
                numbytes = sendto(sockfd, "EOF", 3, 0, (struct sockaddr *)&their_addr, sizeof(their_addr));
                if (numbytes == -1) {
                    perror("sendto");
                    exit(1);
                }
                printf("sent\n");
            }
        } else {                                /* send message */
            numbytes = sendto(sockfd, buf, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, sizeof(their_addr));
            if (numbytes == -1) {
                perror("sendto");
                exit(1);
            }
            printf("Sent: %s",buf);
        }
        numbytes = recvfrom(sockfd, buf, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, &sin_size);

        if (numbytes == -1) {
            perror("recvfrom");
            exit(1);
        }
        if (memcmp(buf, "file", 4) == 0) {      /* receive file */
            char filename[MAXDATASIZE];
            char *p = filename, *q = buf + 5;
            while (*q != '\n') {
                *p++ = *q++;
            }
            FILE *fp = fopen("received_file", "w");
            if (fp == NULL) {
                printf("File opening error.\n");
            } else {
                numbytes = recvfrom(sockfd, buf, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, &sin_size);
                do {
                    if (numbytes == -1) {
                        perror("recvfrom");
                        exit(1);
                    }
                    fwrite(buf, MAXDATASIZE, 1, fp);
                    numbytes = recvfrom(sockfd, buf, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, &sin_size);
                } while(memcmp(buf, "EOF", 3) != 0);
                fclose(fp);
                printf("received file %s\n", filename);
            }

        } else {                                /* receive message */
            buf[numbytes] = '\0';
            printf("Received: %s",buf);
        }
    }
    close(sockfd);

    return 0;
}
