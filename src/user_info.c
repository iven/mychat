/*
 * =====================================================================================
 *
 *       Filename:  user_info.c
 *
 *    Description:  Implemention of user information.
 *
 *        Version:  1.0
 *        Created:  2010年12月18日 01时23分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#include	<stdlib.h> 
#include	<stdio.h> 
#include	<string.h> 
#include	"user_info.h" 

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  user_info_new
 *  Description:  Create a new user information struct.
 * =====================================================================================
 */
    User_info *
user_info_new ( int fd, const char *name )
{
    User_info *info;
    info = (User_info *) calloc(1, sizeof(User_info));
    if (info == NULL) {
        perror("user_info_new");
        return NULL;
    }
    info->fd = fd;
    strncpy(info->name, name, MAX_NAME_LENGTH);
    return info;
}		/* -----  end of function user_info_new  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  user_info_destroy
 *  Description:  Destroy a user information struct.
 * =====================================================================================
 */
    int
user_info_destroy ( User_info *info )
{
    if (info == NULL) {
        return -1;
    }
    free(info);
    return 0;
}		/* -----  end of function user_info_destroy  ----- */

