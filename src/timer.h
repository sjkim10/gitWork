/*****************************************************************************************
timer.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _MY_TIMER_TEST_H_
#define _MY_TIMER_TEST_H_

#include "define.h"
#include "TemplateSingleTon.h"



/*****************************************************************************************
 *                                Extern & Definition
 *****************************************************************************************/

extern volatile bool g_bFlagWakeup;



/*****************************************************************************************
 *                                 cTimer  Class
 *****************************************************************************************/
class cTimer : public TemplateSingleton<cTimer>
{
public:
   cTimer();
   virtual ~cTimer();

   void setupHwTimer(uint8_t uIndexTimer, uint32_t *u32RepeatTime);
   void closeHwTimer(uint8_t uIndexTimer);

   void endHwTimer(uint8_t u8TimerIndex);
   void startHwTimer(uint8_t u8TimerIndex);
   void stopHwTimer(uint8_t u8TimerIndex);

   bool bInitWDT(uint8_t u8Second);
   bool bExitWDT();
   static bool bResetWDT();
   
};


#endif /* _MY_TIMER_TEST_H_ */
