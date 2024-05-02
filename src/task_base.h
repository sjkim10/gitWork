/*****************************************************************************************
task_base.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant

*****************************************************************************************/

#ifndef _TASK_BASE_TEST_H_
#define _TASK_BASE_TEST_H_

#include "sw_feature.h"
#include "define.h"

/*
The "8MB" in Wrover refers to the size of flash, the size of RAM on ESP32 is fixed at 512KB,
roughly 200KB of which is used by IRAM cache/code sections, leaving around 320KB for program memory,
half of which is available for dynamic allocation.
*/



/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

#define MAX_STACK2 2 * 1024
#define MAX_STACK5 5 * 1024
#define MAX_STACK10 10 * 1024
#define MAX_STACK15 15 * 1024
#define MAX_STACK20 20 * 1024



/*****************************************************************************************
 *                              ThreadTask  Class
 *****************************************************************************************/
class ThreadTask
{
public:
   ThreadTask(const char *sName = "", const uint16_t u16SizeStack = 10000, void *pvParam = NULL, int16_t n16Priority = 0, void **pvHandle = NULL, int16_t n16Core = 1, uint8_t u8Tag = 0);

   virtual ~ThreadTask();

   virtual void task_main() = 0;



protected:
   static void task(void *_params);
};

#endif /* _TASK_BASE_TEST_H_ */
