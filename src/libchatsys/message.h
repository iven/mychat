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

enum {
    CHAT_MSG_ERROR,
    CHAT_MSG_ACK,
    CHAT_MSG_LOGIN,
    CHAT_MSG_CHAT,
    CHAT_MSG_LOGOUT,
    CHAT_MSG_HOLD,
};				/* ----------  end of enum  ---------- */

struct chat_msg {
    char version;
    char type;
    char sn;
    int  len;
    int  fd;
    char text[MAX_TEXT_LEN];
};				/* ----------  end of struct chat_msg  ---------- */
typedef struct chat_msg Chat_msg;

struct chat_pdu {
    char version:4;
    char type:4;
    char sn;
    int  fd;
    short len;
    char text[MAX_TEXT_LEN];
};				/* ----------  end of struct chat_pdu  ---------- */
typedef struct chat_pdu Chat_pdu;

Chat_msg *chat_msg_new(void);
int chat_msg_destroy (Chat_msg *msg);

int chat_msg_parse(const Chat_pdu *pdu, Chat_msg *msg);
int chat_msg_format(Chat_pdu *pdu, const Chat_msg *msg);

#endif   /* ----- #ifndef MESSAGE_INC  ----- */
