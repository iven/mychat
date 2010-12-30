/*
 * =====================================================================================
 *
 *       Filename:  msg_queue.h
 *
 *    Description:  Interface of message queue.
 *
 *        Version:  1.0
 *        Created:  2010年12月29日 17时37分21秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#ifndef  MSG_QUEUE_INC
#define  MSG_QUEUE_INC

#include    "queue.h"
#include    "event.h"

struct msg_queue {
    Queue *queue;
    Event *event;
};               /* ----------  end of struct msg_queue  ---------- */
typedef struct msg_queue Chat_msg_queue;

Chat_msg_queue * chat_msg_queue_new ( void );
int chat_msg_queue_destroy ( Chat_msg_queue *msg_queue );
Chat_msg * chat_msg_queue_pop ( Chat_msg_queue *msg_queue );
int chat_msg_queue_push ( Chat_msg_queue *msg_queue, Chat_msg *msg );

#endif   /* ----- #ifndef MSG_QUEUE_INC  ----- */
