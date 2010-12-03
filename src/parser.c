/*
 * =====================================================================================
 *
 *       Filename:  parser.c
 *
 *    Description:  Implemention of ChatSys PDU parser.
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
#include	"parser.h" 

struct chat_pdu {
    char version:4;
    char type:4;
    char sn;
    short len;
    char text[MAX_TEXT_LEN];
};				/* ----------  end of struct chat_pdu  ---------- */

typedef struct chat_pdu Chat_pdu;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_pdu_parse
 *  Description:  Parse pdu to msg.
 * =====================================================================================
 */
    int
chat_pdu_parse (void *pdu, Chat_msg *msg)
{
    Chat_pdu *p = (Chat_pdu *) pdu;
    msg->version = p->version;
    msg->type = p->type;
    msg->sn = p->sn;
    msg->len = p->len;
    memcpy(msg->text, p->text, MAX_TEXT_LEN);
    
    return 0;
}		/* -----  end of function chat_pdu_parse  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chat_pdu_format
 *  Description:  Format msg to pdu.
 * =====================================================================================
 */
    int
chat_pdu_format (void *pdu, Chat_msg *msg)
{
    Chat_pdu *p = (Chat_pdu *) pdu;
    p->version = msg->version;
    p->type = msg->type;
    p->sn = msg->sn;
    p->len = msg->len;
    memcpy(p->text, msg->text, MAX_TEXT_LEN);
    
    return 0;
}		/* -----  end of function chat_pdu_format  ----- */

