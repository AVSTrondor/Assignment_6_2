/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: ui.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "gpio.h"
#include "systick.h"
#include "tmodel.h"
#include "systick.h"
#include "debug.h"
#include "rtc.h"
#include "rtcs.h"
#include "string.h"


/*****************************    Defines    *******************************/


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U i;
INT8U InBuf[128];

/*****************************   Functions   *******************************/


void debug_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    INT8U ch;
    static int i = 0;
    /*static int j = 0;*/

    switch(my_state){
    case DEBUG_WAIT:
        if(get_file(COM1, &ch)){

                    gfprintf(COM1, "Press received: %c \n",ch);

                    if(ch == 'T'){
                        set_state(1);
                        gfprintf(COM1, "Command: TASK STATUS  \n");

                    } else if(ch == 'S'){
                        set_state(2);
                        gfprintf(COM1, "Command: SEM STATUS \n");

                    }else if(ch == 'Q'){
                        set_state(3);
                        gfprintf(COM1, "Command: QUEUE STATUS \n");

                    }
        }
        break;

    case DEBUG_TASK:
        if(queue_empty(Q_UART_TX)){
            if (get_task_sem(i) == 0 && get_task_timer(i) == 0) {
                    gfprintf(COM1, "NR: %02d, TASK: %02d, Condition: %s,                      State: %02d, Event: %01d \r\n",i, get_task_name(i), get_task_condition(i), get_task_state(i), get_task_event(i));
                } else if (get_task_timer(i) == 0) {
                    gfprintf(COM1, "NR: %02d, TASK: %02d, Condition: %s, Sem: %02d,             State: %02d, Event: %01d \r\n",i, get_task_name(i), get_task_condition(i), get_task_sem(i), get_task_state(i), get_task_event(i));
                } else if (get_task_sem(i) == 0) {
                    gfprintf(COM1, "NR: %02d, TASK: %02d, Condition: %s,          Tim: %05d, State: %02d, Event: %01d \r\n",i, get_task_name(i), get_task_condition(i), get_task_timer(i), get_task_state(i), get_task_event(i));
                } else {
                    gfprintf(COM1, "NR: %02d, TASK: %02d, Condition: %s, Sem: %02d, Tim: %05d, State: %02d, Event: %01d \r\n",i, get_task_name(i), get_task_condition(i), get_task_sem(i), get_task_timer(i), get_task_state(i), get_task_event(i));
                }

            if(i >= (MAX_TASKS-1)){
                set_state(0);
                i=0;
            }
            else{
                i=i+1;
            }
        }

        break;


    case DEBUG_SEM:
        if(queue_empty(Q_UART_TX)){
            if(get_sem_count(i)==0){

            }
            else{
                gfprintf(COM1, "NR: %02d, Condition: %02d, Type %02d, Count: %4d \r\n",i, get_sem_condition(i),get_sem_type(i),get_sem_count(i));
            }
            if(i >= (MAX_SEMAPHORES-1)){
                set_state(0);
                i=0;
            }
            else{
                i=i+1;
            }
        }

        break;

    case DEBUG_Q:
            if(queue_empty(Q_UART_TX)){

                if(get_queue_not_full(i)==0 && get_queue_not_empty(i)==0){
                    gfprintf(COM1, "NR: %02d, Head: %4d, Tail %4d \r\n",i,get_queue_head(i),get_queue_tail(i));
                }
                else{
                    gfprintf(COM1, "NR: %02d, Head: %4d, Tail %4d, Queue not full: %02d, Queue not empty: %02d \r\n",i,get_queue_head(i),get_queue_tail(i),get_queue_not_full(i),get_queue_not_empty(i));
                }

                if(i >= (MAX_QUEUES-1)){
                    set_state(0);
                    i=0;
                }
                else{
                    i=i+1;
                }
            }

            break;

        }


}

/****************************** End Of Module *******************************/












