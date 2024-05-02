/*****************************************************************************************
task.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _MY_TASK_TEST_H_
#define _MY_TASK_TEST_H_

#include "sw_feature.h"
//#include <freertos/task.h>
//#include <freertos/semphr.h>
//#include <freertos/queue.h>
#include "TemplateSingleTon.h"


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#define PRO_CORE 0
#define APP_CORE 1

#define MAX_MESSAGE_TASK 100
#define MAX_QUEUE_SIZE 20

#define QueueHandle_t uint8_t* //sjkim

typedef struct _stThreadMessage_t
{
   uint8_t u8TaskID;
   int8_t n8Event;
   uint8_t u8NowOp;
   uint32_t u32Index;
   uint8_t u8Rst;
//   char ucData[MAX_MESSAGE_TASK];
} stThreadMessage_t;
extern stThreadMessage_t g_stThreadMessage;


enum
{
   TASK_ID_BLE_0 = 0,
   TASK_ID_UART_PC_1,
   TASK_ID_I2C_1,
   TASK_ID_OP_1,
   TASK_ID_TESTMODE_1,
};

enum
{
   EVT_GET_SENSOR_DATA = 1,
   EVT_GET_CONFIG_DATA,
   EVT_SET_CONFIG_DATA,
   EVT_SET_TIME_STAMP,
   EVT_RESET_WATCHDOG,
   EVN_RESET,
   EVT_NONE,
};

enum
{
   TASK_MAIN_ID = 1,
   TASK_NETWORK_ID,
   TASK_UART_ID
};

enum
{
  SEMA_NONE = 0,
	SEMA_DONE_GIVE,
	SEMA_DONE_TAKE,
};


/*****************************************************************************************
 *                              cTask  Class
 *****************************************************************************************/
class cTask : public TemplateSingleton<cTask>
{
public:
   cTask();
   virtual ~cTask();

   void CreateTaskClass();
   void CreateAllQueue();
   void CreateAllMutex();
   void CreateTasks();

   // void CreateCoutingSemaphore();
   void SendMessageQueue(char *sTitle, uint8_t u8TaskID, QueueHandle_t Queue, int8_t n8Event, uint8_t u8NowOp, uint32_t u32Index, uint8_t u8Rst);


	void CreateUartPc();
	void SemaGiveUartPc();
	void SemaTakeUartPc(char* pCaller, uint16_t u16WaitTime);

public:   

#if 0
	static TaskHandle_t g_hTaskBle;
	static TaskHandle_t g_hTaskUartPc, g_hTaskI2C, g_hTaskOp, g_hTaskTestMode;

	static QueueHandle_t m_QueueBle;
	static QueueHandle_t m_QueueUartPc;
	static QueueHandle_t m_QueueI2C;
	static QueueHandle_t m_QueueOp;
	static QueueHandle_t m_QueueTestMode;

	static SemaphoreHandle_t m_hSemUartPc;
#endif

	static uint8_t m_u8TakeGive;
	static uint8_t m_u8TakeGive2;
	static uint8_t m_u8TakeGiveFun;
	static uint8_t m_u8TakeGiveUartPC;

};

#endif
