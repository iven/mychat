/*
 * =====================================================================================
 *
 *       Filename:  message.c
 *
 *    Description:  Implementation of ChatSys PDU parser and formater.
 *
 *        Version:  1.0
 *        Created:  2010年12月02日 20时07分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#include    <string.h>
#include    <stdlib.h>
#include    "message.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_msg_new
 *  Description:  Create a new chat message.
 * =====================================================================================
 */
    Chat_msg *
chat_msg_new ( void )
{
    Chat_msg *msg = (Chat_msg *) calloc(1, sizeof(Chat_msg));
    msg->version = CHAT_VERSION;
    return msg;
}       /* -----  end of function chat_msg_new  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_msg_new_from_msg
 *  Description:  Create a new chat message from existing message.
 * =====================================================================================
 */
    Chat_msg *
chat_msg_new_from_msg ( Chat_msg *old_msg )
{
    Chat_msg *msg = (Chat_msg *) calloc(1, sizeof(Chat_msg));
    msg->version = old_msg->version;
    msg->type = old_msg->type;
    msg->sn = old_msg->sn;
    msg->fd = old_msg->fd;
    msg->len = old_msg->len;
    memcpy(msg->text, old_msg->text, MAX_TEXT_LEN);
    return msg;
}       /* -----  end of function chat_msg_new_from_msg  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_msg_destroy
 *  Description:  Destroy a chat message.
 * =====================================================================================
 */
    int
chat_msg_destroy ( Chat_msg *msg )
{
    if (msg == NULL) {
        return -1;
    }
    free(msg);
    return 0;
}       /* -----  end of function chat_msg_destroy  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_msg_parse
 *  Description:  Parse pdu to msg.
 * =====================================================================================
 */
    int
chat_msg_parse (const Chat_pdu *pdu, Chat_msg *msg)
{
    msg->version = pdu->version;
    msg->type = pdu->type;
    msg->sn = pdu->sn;
    msg->fd = 0;
    msg->len = pdu->len;
    memcpy(msg->text, pdu->text, MAX_TEXT_LEN);

    return 0;
}       /* -----  end of function chat_msg_parse  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_msg_format
 *  Description:  Format msg to pdu.
 * =====================================================================================
 */
    int
chat_msg_format (Chat_pdu *pdu, const Chat_msg *msg)
{
    pdu->version = msg->version;
    pdu->type = msg->type;
    pdu->sn = msg->sn;
    pdu->len = msg->len;
    memcpy(pdu->text, msg->text, MAX_TEXT_LEN);

    return 0;
}       /* -----  end of function chat_msg_format  ----- */

