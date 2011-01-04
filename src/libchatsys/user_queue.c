/*
 * =====================================================================================
 *
 *       Filename:  user_queue.c
 *
 *    Description:  Implementation of user queue.
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
#include    "user_queue.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_user_new
 *  Description:  Create a new user struct.
 * =====================================================================================
 */
    Chat_user *
chat_user_new ( int fd, const char *name )
{
    Chat_user *user;
    user = (Chat_user *) calloc(1, sizeof(Chat_user));
    if (user == NULL) {
        perror("chat_user_new");
        return NULL;
    }
    user->fd = fd;
    strncpy(user->name, name, MAX_NAME_LEN);
    return user;
}                                               /* -----  end of function chat_user_new  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_user_destroy
 *  Description:  Destroy a user struct.
 * =====================================================================================
 */
    int
chat_user_destroy ( Chat_user *user )
{
    if (user == NULL) {
        return -1;
    }
    free(user);
    return 0;
}                                               /* -----  end of function chat_user_destroy  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_user_queue_new
 *  Description:  Create an user queue.
 * =====================================================================================
 */
    Chat_user_queue *
chat_user_queue_new ( void )
{
    Chat_user_queue *queue = queue_new();
    return queue;
}                                               /* -----  end of function chat_user_queue_new  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_user_queue_destroy
 *  Description:  Destroy an user queue.
 * =====================================================================================
 */
    int
chat_user_queue_destroy ( Chat_user_queue *queue )
{
    int retval;
    if (queue == NULL) {
        return -1;
    }
    retval = queue_destroy(queue, (QUEUE_DESTROY) chat_user_destroy);
    return retval;
}                                               /* -----  end of function chat_user_queue_destroy  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_user_queue_add
 *  Description:  Add an @user to the queue.
 * =====================================================================================
 */
    int
chat_user_queue_add ( Chat_user_queue *queue, Chat_user *user )
{
    Queue_node *node;
    if (queue == NULL || user == NULL) {
        return -1;
    }
    if ((node = queue_node_new(user)) == NULL) {
        return -2;
    }
    queue_push(queue, node);
    return 0;
}                                               /* -----  end of static function chat_user_queue_add  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_user_queue_remove
 *  Description:  Remove @user from @queue.
 * =====================================================================================
 */
    int
chat_user_queue_remove ( Chat_user_queue *queue, Chat_user *user )
{
    Queue_node *node, *node_next;
    if (queue == NULL || user == NULL) {
        return -1;
    }
    queue_foreach(queue, node) {
        /*-----------------------------------------------------------------------------
         *  When it's the first node
         *-----------------------------------------------------------------------------*/
        if ((queue->tail == node) && (node->data == user)) {
            node_next = node->next;
            queue_node_destroy(node, (QUEUE_DESTROY) chat_user_destroy);
            queue->tail = node_next;
            return 0;
        }
        /*-----------------------------------------------------------------------------
         *  OtherWise
         *-----------------------------------------------------------------------------*/
        if ((node->next != NULL) && (node->next->data == user)) {
            node_next = node->next->next;
            queue_node_destroy(node->next, (QUEUE_DESTROY) chat_user_destroy);
            node->next = node_next;
            return 0;
        }
    }
    return -2;
}                                               /* -----  end of function chat_user_queue_remove  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_user_queue_get_from_fd
 *  Description:  Get user struct from @fd.
 * =====================================================================================
 */
    Chat_user *
chat_user_queue_get_from_fd ( Chat_user_queue *queue, int fd )
{
    Chat_user *user = NULL;
    Queue_node *node;
    if (queue == NULL || fd < 0) {
        return NULL;
    }
    queue_foreach(queue, node) {
        user = (Chat_user *) node->data;
        if (user->fd == fd) {
            return user;
        }
    }
    return NULL;
}                                               /* -----  end of function chat_user_queue_get_from_fd  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_user_queue_send_to_all
 *  Description:  Send @msg to all users on the @queue.
 * =====================================================================================
 */
    int
chat_user_queue_send_to_all ( Chat_user_queue *queue, Chat_msg *msg )
{
    Chat_user *user = NULL;
    Queue_node *node;
    queue_foreach(queue, node) {
        user = (Chat_user *) node->data;
        msg->fd = user->fd;
        chat_msg_push(msg);
    }
    return 0;
}                                               /* -----  end of function chat_user_queue_send_to_all  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_user_queue_get_names
 *  Description:  Get all user names on the @queue.
 * =====================================================================================
 */
    char *
chat_user_queue_get_names ( Chat_user_queue *queue )
{
    char *names;
    Queue_node *node;
    Chat_user *user = NULL;

    if (queue == NULL) {
        return NULL;
    }
    names = (char *) calloc(1, MAX_TEXT_LEN + MAX_NAME_LEN);
    queue_foreach(queue, node) {
        user = (Chat_user *) node->data;
        sprintf(names + strlen(names), "%s ", user->name);
        if (strlen(names) >= MAX_TEXT_LEN) {
            names[MAX_TEXT_LEN - 1] = '\0';
            return names;
        }
    }
    names[strlen(names) - 1] = 0;
    return names;
}                                               /* -----  end of function chat_user_queue_get_names  ----- */
