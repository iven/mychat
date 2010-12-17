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

#define	MAX_NAME_LENGTH 256			/*  */

struct user_info {
    int fd;
    char name[MAX_NAME_LENGTH];
};				/* ----------  end of struct user_info  ---------- */
typedef struct user_info User_info;

User_info * user_info_new ( int fd, const char *name );
int user_info_destroy ( User_info *info );

#endif   /* ----- #ifndef USER_INFO_INC  ----- */
