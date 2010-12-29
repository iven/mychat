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

#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    "protocol.h"
#include    "user_info.h"

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
    strncpy(info->name, name, MAX_NAME_LEN);
    return info;
}       /* -----  end of function user_info_new  ----- */

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
}       /* -----  end of function user_info_destroy  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  user_queue_new
 *  Description:  Create an user information queue.
 * =====================================================================================
 */
    User_queue *
user_queue_new ( void )
{
    User_queue *queue = queue_new();
    return queue;
}       /* -----  end of function user_queue_new  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  user_queue_destroy
 *  Description:  Destroy an user information queue.
 * =====================================================================================
 */
    int
user_queue_destroy ( User_queue *queue )
{
    int retval;
    if (queue == NULL) {
        return -1;
    }
    retval = queue_destroy(queue, (QUEUE_DESTROY) user_info_destroy);
    return retval;
}       /* -----  end of function user_queue_destroy  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  user_queue_add
 *  Description:  Add an user information to the queue.
 * =====================================================================================
 */
    int
user_queue_add ( User_queue *queue, User_info *user_info )
{
    Queue_node *node;
    if (queue == NULL || user_info == NULL) {
        return -1;
    }
    if ((node = queue_node_new(user_info)) == NULL) {
        return -2;
    }
    queue_push(queue, node);
    return 0;
}       /* -----  end of static function user_queue_add  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  user_queue_remove
 *  Description:  Remove @user_info from @queue.
 * =====================================================================================
 */
    int
user_queue_remove ( User_queue *queue, User_info *user_info )
{
    Queue_node *node, *node_next;
    if (queue == NULL || user_info == NULL) {
        return -1;
    }
    queue_foreach(queue, node) {
        /*-----------------------------------------------------------------------------
         *  When it's the first node
         *-----------------------------------------------------------------------------*/
        if ((queue->tail == node) && (node->data == user_info)) {
            node_next = node->next;
            queue_node_destroy(node, (QUEUE_DESTROY) user_info_destroy);
            queue->tail = node_next;
            return 0;
        }
        /*-----------------------------------------------------------------------------
         *  OtherWise
         *-----------------------------------------------------------------------------*/
        if ((node->next != NULL) && (node->next->data == user_info)) {
            node_next = node->next->next;
            queue_node_destroy(node->next, (QUEUE_DESTROY) user_info_destroy);
            node->next = node_next;
            return 0;
        }
    }
    return -2;
}       /* -----  end of function user_queue_remove  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  user_queue_get_from_fd
 *  Description:  Get user information struct from @fd.
 * =====================================================================================
 */
    User_info *
user_queue_get_from_fd ( User_queue *queue, int fd )
{
    User_info *user_info = NULL;
    Queue_node *node;
    if (queue == NULL || fd < 0) {
        return NULL;
    }
    queue_foreach(queue, node) {
        user_info = (User_info *) node->data;
        if (user_info->fd == fd) {
            return user_info;
        }
    }
    return NULL;
}       /* -----  end of function user_queue_get_from_fd  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  user_queue_send_to_all
 *  Description:  Send @msg to all users on the @queue.
 * =====================================================================================
 */
    int
user_queue_send_to_all ( User_queue *queue, Chat_msg *msg )
{
    User_info *user_info = NULL;
    Queue_node *node;
    queue_foreach(queue, node) {
        user_info = (User_info *) node->data;
        msg->fd = user_info->fd;
        chat_msg_push(msg);
    }
    return 0;
}       /* -----  end of function user_queue_send_to_all  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  user_queue_get_names
 *  Description:  Get all user names on the @queue.
 * =====================================================================================
 */
    char *
user_queue_get_names ( User_queue *queue )
{
    char *names;
    Queue_node *node;
    User_info *user_info = NULL;

    if (queue == NULL) {
        return NULL;
    }
    names = (char *) calloc(1, MAX_TEXT_LEN + MAX_NAME_LEN);
    queue_foreach(queue, node) {
        user_info = (User_info *) node->data;
        sprintf(names + strlen(names), "%s ", user_info->name);
        if (strlen(names) >= MAX_TEXT_LEN) {
            names[MAX_TEXT_LEN - 1] = '\0';
            return names;
        }
    }
    names[strlen(names) - 1] = 0;
    return names;
}       /* -----  end of function user_queue_get_names  ----- */
