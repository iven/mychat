/*
 * =====================================================================================
 *
 *       Filename:  parser.h
 *
 *    Description:  Interface of the ChatSys parser.
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

#ifndef  PARSER_INC
#define  PARSER_INC

#define	MAX_TEXT_LEN 1024			/*  */

struct chat_msg {
    char version;
    char type;
    char sn;
    int  len;
    char text[MAX_TEXT_LEN];
};				/* ----------  end of struct chat_msg  ---------- */

typedef struct chat_msg Chat_msg;

int char_pdu_parse(void *pdu, Chat_msg *msg);
int char_pdu_format(void *pdu, Chat_msg *msg);

#endif   /* ----- #ifndef PARSER_INC  ----- */
