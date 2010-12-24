/*
 * =====================================================================================
 *
 *       Filename:  user_info.h
 *
 *    Description:  Interface of user information.
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

#ifndef  USER_INFO_INC
#define  USER_INFO_INC

#include    "queue.h"
#include    "message.h"

#define MAX_NAME_LEN 256            /*  */

struct user_info {
    int fd;
    char name[MAX_NAME_LEN];
};              /* ----------  end of struct user_info  ---------- */
typedef struct user_info User_info;

typedef Queue User_queue;

User_info * user_info_new ( int fd, const char *name );
int user_info_destroy ( User_info *info );

User_queue * user_queue_new ( void );
int user_queue_destroy ( User_queue *queue );
int user_queue_add ( User_queue *queue, User_info *user_info );
int user_queue_remove ( User_queue *queue, User_info *user_info );
User_info * user_queue_get_from_fd ( User_queue *queue, int fd );
int user_queue_send_to_all ( User_queue *queue, Chat_msg *msg );
char * user_queue_get_names ( User_queue *queue );

#endif   /* ----- #ifndef USER_INFO_INC  ----- */
