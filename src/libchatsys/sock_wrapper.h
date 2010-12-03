/*
 * =====================================================================================
 *
 *       Filename:  sock_wrapper.h
 *
 *    Description:  
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

/* #####   HEADER FILE INCLUDES   ################################################### */

/* #####   MACROS  -  LOCAL TO THIS SOURCE FILE   ################################### */
#define	SERVER_PORT 6666			/*  */
#define	MAX_MSG_SIZE 1024			/*  */

/* #####   TYPE DEFINITIONS  -  LOCAL TO THIS SOURCE FILE   ######################### */

/* #####   DATA TYPES  -  LOCAL TO THIS SOURCE FILE   ############################### */

/* #####   VARIABLES  -  LOCAL TO THIS SOURCE FILE   ################################ */

/* #####   PROTOTYPES  -  LOCAL TO THIS SOURCE FILE   ############################### */

/* #####   FUNCTION DEFINITIONS  -  EXPORTED FUNCTIONS   ############################ */
int chat_server_init (void);
int chat_server_exit (void);
int chat_server_accept_client (void);
int chat_server_close_client (int fd);
int chat_server_send (int fd, const char *message, int size);
int chat_server_recv (int fd, char *buf, int size);

int chat_client_init (const char *server_name);
int chat_client_exit (void);
int chat_client_recv (char *buf, int size);
int chat_client_send (const char *message, int size);

/* #####   FUNCTION DEFINITIONS  -  LOCAL TO THIS SOURCE FILE   ##################### */

#endif   /* ----- #ifndef SOCK_WRAPPER_INC  ----- */
