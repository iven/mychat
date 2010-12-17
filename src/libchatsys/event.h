/*
 * =====================================================================================
 *
 *       Filename:  event.h
 *
 *    Description:  Interface of event.
 *
 *        Version:  1.0
 *        Created:  2010年12月04日 08时15分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xu Lijian (ivenvd), ivenvd@gmail.com
 *        Company:  USTC, China
 *
 * =====================================================================================
 */

#ifndef  EVENT_INC
#define  EVENT_INC

#include	<semaphore.h> 

typedef sem_t Event;

Event *event_new(int value);
int event_destroy(Event *event);
int event_post(Event *event);
int event_wait(Event *event);

#endif   /* ----- #ifndef EVENT_INC  ----- */
