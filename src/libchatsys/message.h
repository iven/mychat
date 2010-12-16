/*
 * =====================================================================================
 *
 *       Filename:  message.h
 *
 *    Description:  Interface of the ChatSys message.
 *
 *        Version:  1.0
 *        Created:  2010年12月02日 19时57分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#ifndef  MESSAGE_INC
#define  MESSAGE_INC

#define	MAX_TEXT_LEN 1024			/*  */

struct chat_msg {
    char version;
    char type;
    char sn;
    int  len;
    char text[MAX_TEXT_LEN];
};				/* ----------  end of struct chat_msg  ---------- */
typedef struct chat_msg Chat_msg;

struct chat_pdu {
    char version:4;
    char type:4;
    char sn;
    short len;
    char text[MAX_TEXT_LEN];
};				/* ----------  end of struct chat_pdu  ---------- */
typedef struct chat_pdu Chat_pdu;

int chat_msg_parse(const Chat_pdu *pdu, Chat_msg *msg);
int chat_msg_format(Chat_pdu *pdu, const Chat_msg *msg);

#endif   /* ----- #ifndef MESSAGE_INC  ----- */
