/*****************************************************************************************
task_uart.h
~~~~~~~~~~~~~~~~~~~~~~~~~

@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _TASK_UART_TEST_H_
#define _TASK_UART_TEST_H_

#include "define.h"
#include "task_base.h"
#include "task.h"
#include "uart.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*                Extern                                                                */
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*                Function                                                              */
/*--------------------------------------------------------------------------------------*/
class cTaskUart : public ThreadTask
{
public:
   cTaskUart(const char *sName = "", uint16_t u16SizeStack = 10000, void *pvParam = NULL, int16_t n16Priority = 0, void **pvHandle = NULL, int16_t n16Core = 1);

   virtual ~cTaskUart();
   virtual void task_main();
   void Task_Uart();

   void start();
   void show(uint32_t u32Index);
   bool bGetRxPacketFromRxData(uint8_t *pRxRawBuf, uint16_t u16RxRawSize, bool *pbTsmCommand);
   uint8_t uProcessUart(uint8_t uNowOp);
   int8_t nDoProcessUart(stThreadMessage_t *pstMessage);
   static int nSerialEvent();

private:
   static uint8_t m_u8RxPacketBuf[MAX_RX_BUFFER_PC];
   static uint8_t m_uRxBufUart[MAX_RX_BUFFER_PC];
   static uint16_t m_u16RxPacketBufSize;
   static uint16_t m_u16RxBytesUart;
private:
   volatile bool m_bStart;


   char m_sTaskName[50];
   uint16_t m_u16SizeStack;
   int16_t m_n16Priority;
   void *m_pvHandle;
   int16_t m_n16Core;
   uint16_t m_u16RxBytesSamd;
};

#endif /* _TASK_UART_TEST_H_ */
