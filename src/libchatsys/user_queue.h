/*
 * =====================================================================================
 *
 *       Filename:  user_queue.h
 *
 *    Description:  Interface of user queue.
 *
 *        Version:  1.0
 *        Created:  2010年12月18日 01时25分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#ifndef  USER_QUEUE_INC
#define  USER_QUEUE_INC

#include    "constants.h"
#include    "queue.h"
#include    "message.h"

struct chat_user {
    int fd;
    char name[MAX_NAME_LEN];
};              /* ----------  end of struct chat_user  ---------- */
typedef struct chat_user Chat_user;

typedef Queue Chat_user_queue;

Chat_user * chat_user_new ( int fd, const char *name );
int chat_user_destroy ( Chat_user *user );

Chat_user_queue * chat_user_queue_new ( void );
int chat_user_queue_destroy ( Chat_user_queue *queue );

int chat_user_queue_add ( Chat_user_queue *queue, Chat_user *user );
int chat_user_queue_remove ( Chat_user_queue *queue, Chat_user *user );

Chat_user * chat_user_queue_get_from_fd ( Chat_user_queue *queue, int fd );
int chat_user_queue_send_to_all ( Chat_user_queue *queue, Chat_msg *msg );
char * chat_user_queue_get_names ( Chat_user_queue *queue );

#endif   /* ----- #ifndef USER_QUEUE_INC  ----- */

