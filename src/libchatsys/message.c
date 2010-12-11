/*
 * =====================================================================================
 *
 *       Filename:  message.c
 *
 *    Description:  Implemention of ChatSys PDU parser and formater.
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

#include	<string.h>
#include	"message.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_pdu_parse
 *  Description:  Parse pdu to msg.
 * =====================================================================================
 */
    int
chat_pdu_parse (const Chat_pdu *pdu, Chat_msg *msg)
{
    msg->version = pdu->version;
    msg->type = pdu->type;
    msg->sn = pdu->sn;
    msg->len = pdu->len;
    memcpy(msg->text, pdu->text, MAX_TEXT_LEN);

    return 0;
}		/* -----  end of function chat_pdu_parse  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_pdu_format
 *  Description:  Format msg to pdu.
 * =====================================================================================
 */
    int
chat_pdu_format (Chat_pdu *pdu, const Chat_msg *msg)
{
    pdu->version = msg->version;
    pdu->type = msg->type;
    pdu->sn = msg->sn;
    pdu->len = msg->len;
    memcpy(pdu->text, msg->text, MAX_TEXT_LEN);

    return 0;
}		/* -----  end of function chat_pdu_format  ----- */

