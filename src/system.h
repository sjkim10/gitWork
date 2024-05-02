/*****************************************************************************************
system.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _SYSTEM_TEST_H_
#define _SYSTEM_TEST_H_

#include "define.h"
#include "TemplateSingleTon.h"

#if 0
The offset represents the partition address in the SPI flash
, which sector size is 0x1000 (4 KB). 
Thus, the offset must be a multiple of 4 KB.
Partitions of type app have to be placed at offsets aligned to 0x10000 (64 K).

We must have the first partition offset to 0x9000. 
We can just use size past there if we prefer. 
The total space on a 16M device is 0x1000000. 
So, 0x1000000 - 0x9000 = 0xFF7000 left. 
Subtract 0x5000 for the nvs partition leaves 0xFF2000.
#endif


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#define MAIN_CPU_CLOCK 240 /* Default : 240MHz */
#define WAKEUP_TIME		 3000L


enum
{
	EXT0_TYPE = 0,
	EXT1_TYPE
};


/*****************************************************************************************
 *                              cSystem  Class
 *****************************************************************************************/
class cSystem : public TemplateSingleton<cSystem>
{
public:
	cSystem();
	virtual ~cSystem();
	void initSystem();
	void configADC();

	static uint16_t u16GetBatteryLevel();
	void initRTC();
	void print_reset_reason(uint8_t /*RESET_REASON*/ reason);
	void print_wakeup_reason();
	void PeriodDeepSleep(uint8_t nSec);
	void gotoDeepSleep();
#if 0	 
	void setWakeupTimer(uint8_t nSec);
	void setWakeupExt(uint8_t uExt);
#else	 
	void gotoSleep();
  void setWakeupRTC();
	static void doWakeupRTC();
	
	void setWakeupGpio(uint8_t u8GpioPin/*9*/);

#endif
};

#endif /* _SYSTEM_TEST_H_ */
