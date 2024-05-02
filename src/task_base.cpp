/*****************************************************************************************
  task_base.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-02-24
  @ Author : Piquant
*****************************************************************************************/
#include <Arduino.h> // using Arduino development platform
//#include <driver/uart.h>
//#include <freertos/FreeRTOS.h>
//#include <freertos/task.h>
#include "task_base.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------
  function : cTask()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
ThreadTask::ThreadTask(const char *sName, const uint16_t u16SizeStack, void *pvParam, int16_t n16Priority, void **pvHandle, int16_t n16Core, uint8_t u8Tag )
{
#if 0
   xTaskCreatePinnedToCore(
       task,         /* Task function. */
       sName,        /* name of task. */
       u16SizeStack, /* Stack size of task */
       pvParam,      /* parameter of the task */
       n16Priority,  /* priority of the task */
       pvHandle,     /* Task handle to keep track of created task */
       n16Core       /* pin task to core 0 */
   );
#endif
}


/*---------------------------------------------------------------------------------------
  function : ~~ThreadTask()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
ThreadTask::~ThreadTask()
{
}

/*---------------------------------------------------------------------------------------
  function : task()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void ThreadTask::task(void *_params)
{
	if(_params == NULL) return;
   ThreadTask *p = static_cast<ThreadTask *>(_params);
   p->task_main();
}

