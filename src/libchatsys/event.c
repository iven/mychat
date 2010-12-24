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

#include    <stdio.h>
#include    <stdlib.h>
#include    "event.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  event_new
 *  Description:  Create a new event.
 * =====================================================================================
 */
    Event *
event_new ( int value )
{
    Event *event = (Event *) calloc(1, sizeof(Event));
    if (event == NULL || sem_init(event, 0, value) < 0) {
        perror("event_init");
        return NULL;
    }
    return event;
}       /* -----  end of function event_new  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  event_destroy
 *  Description:  Destroy an event.
 * =====================================================================================
 */
    int
event_destroy ( Event *event )
{
    if (event == NULL) {
        return -1;
    }
    sem_destroy(event);
    free(event);
    return 0;
}       /* -----  end of function event_destroy  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  event_post
 *  Description:  Post a event.
 * =====================================================================================
 */
    int
event_post ( Event *event )
{
    if (event == NULL) {
        return -1;
    }
    return sem_post(event);;
}       /* -----  end of function event_post  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  event_wait
 *  Description:  Wait a event.
 * =====================================================================================
 */
    int
event_wait ( Event *event )
{
    if (event == NULL) {
        return -1;
    }
    return sem_wait(event);
}       /* -----  end of function event_wait  ----- */
