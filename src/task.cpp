/*****************************************************************************************
  test.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-02-24
  @ Author : Piquant
*****************************************************************************************/
#include <Arduino.h> // using Arduino development platform
//#include <driver/uart.h>
//#include <FreeRTOS.h>
//#include <task.h>
//#include <semph.h>

#include "system.h"
#include "config.h"
#include "utils.h"
#include "opmode.h"
#include "print.h"
#include "timer.h"
#include "task.h"
#include "init.h"


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#if 0
QueueHandle_t cTask::m_QueueBle = NULL;
QueueHandle_t cTask::m_QueueUartPc = NULL;
QueueHandle_t cTask::m_QueueI2C = NULL;
QueueHandle_t cTask::m_QueueOp = NULL;
QueueHandle_t cTask::m_QueueTestMode = NULL;

SemaphoreHandle_t cTask::m_hSemUartPc = NULL;

/* Tasks for PRO_CORE (Core0) */
TaskHandle_t cTask::g_hTaskBle ;
/* Tasks for APP_CODE (Core0) */
TaskHandle_t cTask::g_hTaskUartPc, cTask::g_hTaskI2C, cTask::g_hTaskOp, cTask::g_hTaskTestMode;
#endif

uint8_t cTask::m_u8TakeGive = SEMA_NONE;
uint8_t cTask::m_u8TakeGive2 = SEMA_NONE;
uint8_t cTask::m_u8TakeGiveUartPC = SEMA_NONE;
uint8_t cTask::m_u8TakeGiveFun = SEMA_NONE;

/*---------------------------------------------------------------------------------------
  function : cTask()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cTask::cTask()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cTask()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cTask::~cTask()
{
}

/*---------------------------------------------------------------------------------------
  function : CreateTaskClass()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTask::CreateTaskClass()
{
	int16_t T_PRIORITY = 1; /*IDLE_PRIORITY*/


#if 0
   cTaskOp *p9 = new cTaskOp{"cTaskOp", 10 * 1024, NULL, T_PRIORITY, &g_hTaskOp, APP_CORE};
   if (p9 == NULL)
   {
      printk(LOG_ERROR, F("<Error>  new cTaskOp()\n"));
   }
   else
   {
      g_stInst.pVoidOpMode = (void *)p9;
   }
   printk(LOG_TRACE, F("[CreateTaskClass:cTaskOp]cTaskOp Free internal heap : %u, %u\n"), ESP.getFreeHeap(), ESP.getFreePsram());
#endif

#if 0
   cBleDev*p10 = new cBleDev{"cBleDev", 10000, NULL, T_PRIORITY, &g_hTaskBle, APP_CORE};   
   if(p10 == NULL) 
   {
      printk(LOG_ERROR, F("<Error>  new cBleDev()\n"));
   }
   else {
      g_stInst.pVoidBle = (void *)p10;
   }
#endif
}

/*---------------------------------------------------------------------------------------
  function : CreateAllQueue()
    @brief    : Q Set for free RTOS
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTask::CreateAllQueue()
{
#if 0
   if ((m_QueueUartPc = xQueueCreate(MAX_QUEUE_SIZE, sizeof(stThreadMessage_t))) == NULL)
   {
      printk(LOG_ERROR, F("<Error> [%s] Creating the Queue\n"), "m_QueueUartPc");
   }
   if ((m_QueueOp = xQueueCreate(MAX_QUEUE_SIZE, sizeof(stThreadMessage_t))) == NULL)
   {
      printk(LOG_ERROR, F("<Error> [%s] Creating the Queue\n"), "m_QueueOp");
   }
#endif	 
   printk(LOG_DEBUG, F("CreateAllQueue()\n"));
}

/*---------------------------------------------------------------------------------------
  function : CreateMutex()
    @brief    : Initialize of Mutex
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTask::CreateAllMutex()
{
	CreateUartPc();
}

/*---------------------------------------------------------------------------------------
  function : CreateTasks()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTask::CreateTasks()
{
   CreateAllQueue();
   CreateAllMutex();

   CreateTaskClass();

#if 0
	/* Init Ap Socket */
	((cTask *)g_stInst.pTask)->SendMessageQueue((char *)"m_QueueApSocket"
		, TASK_ID_APSOCKET_0, cTask::m_QueueApSocket, EVT_INIT_AP_SOCKET, OP_AP_SOCKET_INIT, 0, 0);
#endif		
}


/*---------------------------------------------------------------------------------------
  function : SendMessageQueue()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTask::SendMessageQueue(char *sTitle, uint8_t u8TaskID, QueueHandle_t Queue, int8_t n8Event, uint8_t u8NowOp, uint32_t u32Index, uint8_t u8Rst)
{
#if 0
   static stThreadMessage_t _stMessage = {0};
   _stMessage.u8TaskID = u8TaskID;
   _stMessage.n8Event = n8Event;
   _stMessage.u8NowOp = u8NowOp;
   _stMessage.u32Index = u32Index;
   _stMessage.u8Rst = u8Rst;


   printk(LOG_TRACE, F("SendMessageQueue(%s, %u, %d, %u, %u)\n"), sTitle, u8TaskID, n8Event, u8NowOp, u32Index);

   if(xQueueSend(Queue, (const void *)&_stMessage, (TickType_t)0)!= pdPASS)   
   {
		printk(LOG_ALWAYS, F("<Error>SendMessageQueue(%s, %u, %d, %u, %u)\n"), sTitle, u8TaskID, n8Event, u8NowOp, u32Index);
	   return;
   }
#endif
}



/*---------------------------------------------------------------------------------------
  function : CreateUartPc()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTask::CreateUartPc()
{
#if 0
   if (cTask::m_hSemUartPc == NULL)
   {
		/* Create the semaphore to guard a shared resource. */
		cTask::m_hSemUartPc = xSemaphoreCreateBinary();
		//cTask::m_hSemUartSamdProcess = xSemaphoreCreateMutex();
		printk(LOG_DEBUG, F("CreateUartPc() xSemaphoreCreateMutex(m_hSemUartPc = %p)\n")
		    , cTask::m_hSemUartPc);				

#if 0
		if(xSemaphoreGive(cTask::m_hSemUartPc) == pdTRUE)
		{
		   printk(LOG_TRACE, F("<TRUE>xSemaphoreGive(cTask::m_hSemUartPc)\n"));	 
		}
		else
		{
		   printk(LOG_ERROR, F("<FALSE>xSemaphoreGive(cTask::m_hSemUartPc)\n"));   
		}			
#endif		
   }
#endif	 
}

/*---------------------------------------------------------------------------------------
  function : SemaGiveUartPc()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTask::SemaGiveUartPc()
{
#if 0
	if(cTask::m_u8TakeGiveUartPC == SEMA_DONE_GIVE) return;
	
	if(xSemaphoreGive(cTask::m_hSemUartPc) == pdTRUE)
	{
		printk(LOG_TRACE, F("<TRUE>SemaGiveUartPc(cTask::m_hSemUartPc)\n"));	 
	}
	else
	{
		printk(LOG_ERROR, F("<FALSE>SemaGiveUartPc(cTask::m_hSemUartPc)\n"));	 
	}
	cTask::m_u8TakeGiveUartPC = SEMA_DONE_GIVE;	
#endif	
}

/*---------------------------------------------------------------------------------------
  function : SemaTakeUartPc()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTask::SemaTakeUartPc(char* pCaller, uint16_t u16WaitTime)
{
#if 0
	if(cTask::m_u8TakeGiveUartPC == SEMA_DONE_TAKE)
	{
		printk(LOG_ERROR, F("<Error>%s SemaTakeUartPc() m_u8TakeGiveUartPC == SEMA_DONE_TAKE\n"), pCaller);	 
		return;
	}

	if(xSemaphoreTake(cTask::m_hSemUartPc, u16WaitTime) == pdTRUE)
	{
	  printk(LOG_TRACE, F("<TRUE>%s SemaTakeUartPc(cTask::m_hSemUartPc, %u)\n"), pCaller, u16WaitTime);
	}
	else
	{
	  printk(LOG_ERROR, F("<FALSE>%s SemaTakeUartPc(cTask::m_hSemUartPc, %u)\n"), pCaller, u16WaitTime);
	}
	cTask::m_u8TakeGiveUartPC = SEMA_DONE_TAKE;
#endif	
}

