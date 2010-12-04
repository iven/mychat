/*
 * =====================================================================================
 *
 *       Filename:  event.c
 *
 *    Description:  Implement of event.
 *
 *        Version:  1.0
 *        Created:  2010年12月04日 08时18分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#include	<stdio.h> 
#include	"event.h" 

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  event_init
 *  Description:  Initialize a event.
 * =====================================================================================
 */
    int
event_init ( Event *event, int value )
{
    if (sem_init(event, 0, value) < 0) {
        perror("sem_init");
    }
    return 0;
}		/* -----  end of function event_init  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  event_exit
 *  Description:  Event exit.
 * =====================================================================================
 */
    int
event_exit ( Event *event )
{
    sem_destroy(event);
    return 0;
}		/* -----  end of function event_exit  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  event_post
 *  Description:  Post a event.
 * =====================================================================================
 */
    int
event_post ( Event *event )
{
    sem_post(event);
    return 0;
}		/* -----  end of function event_post  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  event_wait
 *  Description:  Wait a event.
 * =====================================================================================
 */
    int
event_wait ( Event *event )
{
    sem_wait(event);
    return 0;
}		/* -----  end of function event_wait  ----- */
