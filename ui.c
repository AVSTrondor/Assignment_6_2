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
#include "ui.h"
#include "rtc.h"
#include "rtcs.h"
#include "string.h"


/*****************************    Defines    *******************************/


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U i;
INT8U InBuf[128];

/*****************************   Functions   *******************************/


void ui_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    INT8U ch;
    static int i = 0;
    static int j = 0;

    switch(my_state){
    case 0:
        if(get_file(COM1, &ch))
        {
                    gfprintf(COM1, "Press received: %c \n",ch);
                    if(ch == 's')
                    {

                        set_state(1);
                        gfprintf(COM1, "Status command: received \n");
                        gfprintf(COM1, "Next State: 1 \n \n");
                        wait(100);
                    }
        }
        break;
    case 1:

        gfprintf(COM1, "Case 1 start \n");

        put_queue(Q_DEBUG,get_name(i),WAIT_FOREVER);
        put_queue(Q_DEBUG,get_condition(i),WAIT_FOREVER);
        put_queue(Q_DEBUG,get_sem(i),WAIT_FOREVER);
        put_queue(Q_DEBUG,get_timer(i),WAIT_FOREVER);
        put_queue(Q_DEBUG,get_state(i),WAIT_FOREVER);
        put_queue(Q_DEBUG,get_event(i),WAIT_FOREVER);

        gfprintf(COM1, "i: %d \n", i);

        i++;

        if(i >= 15){
            gfprintf(COM1, "Next State: 2 \n \n");
            wait(100);
            set_state(2);
            i=0;
            break;
        }

        gfprintf(COM1, "Case 1 done \n \n");
        wait(100);

        break;


    case 2:

        gfprintf(COM1, "Case 2 start \n");
        wait(100);
        get_queue(Q_DEBUG, &ch, WAIT_FOREVER);
        gfprintf(COM1, "%d \n", ch);
        gfprintf(COM1, "J: %d \n", j);
        wait(100);

        gfprintf(COM1, "j: %d \n", j);
        j++;
        if(j >= 15){
            gfprintf(COM1, "Next State: 0 \n \n");
            wait(100);
            set_state(0);
            break;
            j=0;}

        gfprintf(COM1, "Case 2 done \n \n");
        wait(100);

        break;
    }
}

void ui_key_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  INT8U ch;
  static INT8U Buf[6];

  if( get_file( COM3, &ch ))                            // if input received from keyboard (COM3)
  {
    switch( my_state )
    {
    case 0:
      Buf[0] = ch;                                      // add key press to buffer
      gfprintf( COM2, "%c%c%c", 0x1B, 0xC4, ch );       // print the key to the display (COM2) on the second line
      set_state( 1 );
      break;
    case 1:
      Buf[1] = ch;                                      // repeat previous step for all digits of the clock
      gfprintf( COM2, "%c%c%c:", 0x1B, 0xC5, ch );
      set_state( 2 );
      break;
    case 2:
      Buf[2] = ch;
      gfprintf( COM2, "%c%c%c", 0x1B, 0xC7, ch );
      set_state( 3 );
      break;
    case 3:
      Buf[3] = ch;
      gfprintf( COM2, "%c%c%c:", 0x1B, 0xC8, ch );
      set_state( 4 );
      break;
    case 4:
      Buf[4] = ch;
      gfprintf( COM2, "%c%c%c", 0x1B, 0xCA, ch );
      set_state( 5 );
      break;
    case 5:
      Buf[5] = ch;                                      // when all digits received, set the clock
      set_hour( (Buf[0]-'0')*10+Buf[1]-'0');
      set_min( (Buf[2]-'0')*10+Buf[3]-'0');
      set_sec( (Buf[4]-'0')*10+Buf[5]-'0');
      gfprintf( COM2, "%c%c        ", 0x1B, 0xC4 );
      set_state( 0 );
      break;
    }
  }
}


/****************************** End Of Module *******************************/












