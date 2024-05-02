/*****************************************************************************************
  task_uart.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-02-24
  @ Author : Piquant
*****************************************************************************************/



#include <Arduino.h>
//#include <driver/uart.h>

#include "opmode.h"
#include "timer.h"
#include "system.h"
#include "utils.h"
#include "packet.h"
//#include "test.h"

#include "time_func.h"
#include "task.h"
#include "task_uart.h"
#include "init.h"

#include "print.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#define MIN_LEN_COMMAD 5

volatile bool g_bRxUart1Complete = false;



uint8_t cTaskUart::m_u8RxPacketBuf[MAX_RX_BUFFER_PC] = {0};
uint8_t cTaskUart::m_uRxBufUart[MAX_RX_BUFFER_PC] = {0};
uint16_t cTaskUart::m_u16RxPacketBufSize;
uint16_t cTaskUart::m_u16RxBytesUart = 0;

/*---------------------------------------------------------------------------------------
  function : cTaskSamd()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cTaskUart::cTaskUart(const char *sName, uint16_t u16SizeStack, void *pvParam, int16_t n16Priority, void **pvHandle, int16_t n16Core)
    : ThreadTask{sName, u16SizeStack, this, n16Priority, pvHandle, n16Core, 1}
{
   sprintf(m_sTaskName, "%s", sName);
   m_u16SizeStack = u16SizeStack;
   m_n16Priority = n16Priority;
   m_pvHandle = pvHandle;
   m_n16Core = n16Core;

   m_bStart = false;
}

/*---------------------------------------------------------------------------------------
  function : ~cTaskSamd()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cTaskUart::~cTaskUart()
{
   printk(LOG_TRACE, F("[%s] ~cTaskUart()\n"), m_sTaskName);
//   vTaskDelete(NULL);
}

/*---------------------------------------------------------------------------------------
  function : start()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTaskUart::start()
{
   // m_bStart = true;
   // g_u16RxBytesSamd = 0;
   //  printk(LOG_ERROR, F("[Task_UartSamd] start = %u\n"), m_bStart);
}

/*---------------------------------------------------------------------------------------
  function : show()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTaskUart::show(uint32_t u32Index)
{
   static uint32_t u32Count = 0;

   printk(LOG_TRACE, F("show [%s] u32Index = %u, nCount = %u\n"), m_sTaskName, u32Index, u32Count++);
}

/*---------------------------------------------------------------------------------------
  function : task_main()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTaskUart::task_main()
{
   printk(LOG_TRACE, F("[%s] task_main()\n"), m_sTaskName);

   Task_Uart();
}

/*---------------------------------------------------------------------------------------
  function : nDoProcessUart()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
int8_t cTaskUart::nDoProcessUart(stThreadMessage_t *pstMessage)
{
   uint8_t u8NowOp = pstMessage->u8NowOp;

   printk(LOG_ALWAYS, F("<TaskUart> %u\n"), pstMessage->u8TaskID);

   uProcessUart(u8NowOp);

   return RST_SUCCESS;
}

/*---------------------------------------------------------------------------------------
  function : bSerialEvent()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
int cTaskUart::nSerialEvent()
{
   static bool _bStartFlag = false;
   bool bEndFlag = false;
   static uint8_t _u8Start = 0;
   uint16_t i, k;

   while (Serial.available())
   {
      /* add it to the buffer: */
      cTaskUart::m_uRxBufUart[m_u16RxBytesUart] = (char)Serial.read();
      printk(LOG_DEBUG, F("[%u] %02x\n"), cTaskUart::m_u16RxBytesUart, cTaskUart::m_uRxBufUart[cTaskUart::m_u16RxBytesUart]);
      i = m_u16RxBytesUart;

      if(_bStartFlag != true)
      {
         if (cTaskUart::m_uRxBufUart[i] == P2_STX[2]) /* Start Flag */
         {
            if(i >= 2)         
            {
               _bStartFlag = false;
               if ((cTaskUart::m_uRxBufUart[i-2] == P2_STX[0]) && (cTaskUart::m_uRxBufUart[i-1] == P2_STX[1]))
               {
                  _bStartFlag = true;
                  _u8Start = i - 2;
               }        
            }
         }
         if (cTaskUart::m_uRxBufUart[i] == CR)  /* End Flag */         
         {
           	printk(LOG_DEBUG, F("(1) m_u16RxBytesUart = %u -> 0\n"), m_u16RxBytesUart);         
            m_u16RxBytesUart = 0;
            _bStartFlag = false;
            i = 0;
         }
      }
      if(bEndFlag != true)
      {
         if (cTaskUart::m_uRxBufUart[i] == CR)  /* End Flag */
         {
				 printk(LOG_DEBUG, F("bStartFlag = %u, m_u16RxBytesUart = %u, _u8Start = %u, i = %u\n"), _bStartFlag, m_u16RxBytesUart, _u8Start, i);
				 printk(LOG_DEBUG, F("%c(%x), %c(%x)\n"), cTaskUart::m_uRxBufUart[_u8Start+3], cTaskUart::m_uRxBufUart[_u8Start+3], cTaskUart::m_uRxBufUart[_u8Start+4], cTaskUart::m_uRxBufUart[_u8Start+4]);

             if(i >= 3)
             {
                if ((cTaskUart::m_uRxBufUart[i - 3] != CR_CHANGE[0]) && (cTaskUart::m_uRxBufUart[i - 2] != CR_CHANGE[1]) && (cTaskUart::m_uRxBufUart[i - 1] != CR_CHANGE[2]))                
                {  /* For String Communication */
                   if(_bStartFlag && (m_u16RxBytesUart - _u8Start >= 5)) /* TSMDR : 5 */
                   {
                      if (isalpha(cTaskUart::m_uRxBufUart[_u8Start+3]) && (isalpha(cTaskUart::m_uRxBufUart[_u8Start+4]) || isdigit(cTaskUart::m_uRxBufUart[_u8Start+4])))
                      {
                         _u8Start = 0;
                         bEndFlag = true;
					          printk(LOG_DEBUG, F("bEndFlag = true\n"));                                                           
                      }
                   }
                   else
                   {
							 m_u16RxBytesUart = 0;
							 _bStartFlag = false;
                   }
                }
            }
          	if( !_bStartFlag )
          	{
             	m_u16RxBytesUart = 0;
             	_bStartFlag = false;
				}
         }         
      }
      
#if 0      
      if(m_u16RxBytesUart < 4) /* TSMDR */
      {
         printk(LOG_ERROR, F("(2) u8Start = %u, %u, %u, %u\n"), cTaskUart::m_uRxBufUart[0], cTaskUart::m_uRxBufUart[1], cTaskUart::m_uRxBufUart[2], cTaskUart::m_uRxBufUart[3]);
         
         m_u16RxBytesUart++;
         return RST_CONTINUE;
      }
#endif      
      if(bEndFlag)
      {
//         noInterrupts();
         g_bRxUart1Complete = true;
//         interrupts();

         m_u16RxBytesUart++;

         DebugDumpHex(LOG_TRACE, "(0)cTaskUart::m_uRxBufUart", (uint8_t *)cTaskUart::m_uRxBufUart, m_u16RxBytesUart, 40);         

         k = 0;
         for (i = _u8Start; i < m_u16RxBytesUart; i++)
         {
            cTaskUart::m_uRxBufUart[k] = cTaskUart::m_uRxBufUart[i];
            k++;
         }
         
         m_u16RxBytesUart = k;
      
         DebugDumpHex(LOG_TRACE, "(1)cTaskUart::m_uRxBufUart", (uint8_t *)cTaskUart::m_uRxBufUart, m_u16RxBytesUart, 40);         
         cTaskUart::m_uRxBufUart[m_u16RxBytesUart] = 0x0a;
         DebugDumpHex(LOG_DEBUG, "(2)cTaskUart::m_uRxBufUart", (uint8_t *)cTaskUart::m_uRxBufUart, m_u16RxBytesUart, 40);                  
         printk(LOG_TRACE, F("\n<PC Uart End> m_u16RxBytesUart = %u\n"), m_u16RxBytesUart);

         return RST_SUCCESS;            
      }
      
      m_u16RxBytesUart++;
   }

   return RST_CONTINUE;
}

/*---------------------------------------------------------------------------------------
  function : bGetRxPacketFromRxData()
    @brief   : find out the Start bits from RxData
    @param
    @return
----------------------------------------------------------------------------------------*/
bool cTaskUart::bGetRxPacketFromRxData(uint8_t *pRxRawBuf, uint16_t u16RxRawSize, bool *pbTsmCommand)
{
   uint16_t i, k;
   bool bStartFlag = false;
   bool bGotRxMsg = false;
   uint16_t u16StartNo = 0;

   *pbTsmCommand = false;

   if (MAX_RX_BUFFER_PC - m_u16RxPacketBufSize > (int)u16RxRawSize)
   {
      if (bStartFlag != true)
      {
         for (i = 0; i < u16RxRawSize - 2; i++)
         {
            if ((pRxRawBuf[i] == P_STX[0]) && (pRxRawBuf[i + 1] == P_STX[1]) && (pRxRawBuf[i + 2] == P_STX[2])) /* STM */
            {
               u16StartNo = i;
               bStartFlag = true;
               break;
            }
            if ((pRxRawBuf[i] == P2_STX[0]) && (pRxRawBuf[i + 1] == P2_STX[1]) && (pRxRawBuf[i + 2] == P2_STX[2])) /* TSM */
            {
               u16StartNo = i;
               bStartFlag = true;
               *pbTsmCommand = true;
               // printk(LOG_DEBUG, F("[UART] TSM : %u\n"), u16StartNo);
               break;
            }
         }
      }
      printk(LOG_TRACE, F("bStartFlag = %u, pbTsmCommand = %u, u16StartNo = %u, u16RxRawSize = %u\n"), bStartFlag, *pbTsmCommand, u16StartNo, u16RxRawSize);

      m_u16RxPacketBufSize = 0;

      if (bStartFlag == true)
      {
         for (k = u16StartNo; k < u16RxRawSize; k++)
         {
            cTaskUart::m_u8RxPacketBuf[m_u16RxPacketBufSize] = pRxRawBuf[k];

            if (*pbTsmCommand == true)
            {
               if (pRxRawBuf[k] == CR) /* Only ASCII */
               {
                  bGotRxMsg = true;
                  m_u16RxPacketBufSize++;
                  if (k > u16StartNo + 5)
                  {
                     cTaskUart::m_u8RxPacketBuf[m_u16RxPacketBufSize - 1] = CR;
                  }
                  cTaskUart::m_u8RxPacketBuf[m_u16RxPacketBufSize] = 0x0;
                  printk(LOG_TRACE, F("[UART] size = %u\n"), m_u16RxPacketBufSize);
                  DebugDumpHex(LOG_TRACE, NULL, cTaskUart::m_u8RxPacketBuf, m_u16RxPacketBufSize);
                  break;
               }
            }
            else
            {
               if (k >= 3)
               {
                  // if ((pRxRawBuf[k - 4] == P_ETX[0]) && (pRxRawBuf[k - 3] == P_ETX[1]) && (pRxRawBuf[k - 2] == P_ETX[2]) && (pRxRawBuf[k - 1] == CR) && (pRxRawBuf[k] == LF))
                  if ((pRxRawBuf[k - 3] == P_ETX[0]) && (pRxRawBuf[k - 2] == P_ETX[1]) && (pRxRawBuf[k - 1] == P_ETX[2]) && (pRxRawBuf[k] == CR))
                  {
                     bGotRxMsg = true;
                     // g_u16RxPacketBufSize -= 1; /* remove CR and LF */
                     cTaskUart::m_u8RxPacketBuf[m_u16RxPacketBufSize++] = 0x0;
                     printk(LOG_TRACE, F("[UART2] size = %u\n"), m_u16RxPacketBufSize);
                     break;
                  }
               }
               printk(LOG_TRACE, F("<%u>%02x\n"), m_u16RxPacketBufSize, cTaskUart::m_u8RxPacketBuf[m_u16RxPacketBufSize]);
            }
            m_u16RxPacketBufSize++;
         }
         if (k == u16RxRawSize)
            u16StartNo = u16RxRawSize - 1;
      }
   }

   /* Clear the variables realte to Raw UART Data */
   // g_u16RxBytes = 0;

   return bGotRxMsg;
}

/*---------------------------------------------------------------------------------------
  function : uProcessUart()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
uint8_t cTaskUart::uProcessUart(uint8_t uNowOp)
{
   uint8_t u8NextOp = uNowOp; // OP_NONE;
#if 0	 
   bool bRst = false;
   bool bTsmCommand = false;
   uint16_t wNewSize = 0;
   uint8_t *pRemoveCharBuf = NULL;

   if (nSerialEvent() != RST_SUCCESS)
      return uNowOp;

   if (g_bRxUart1Complete)
   {
//      noInterrupts();
      g_bRxUart1Complete = false;
//      interrupts();

      if (m_u16RxBytesUart < MIN_LEN_COMMAD)
         return OP_NONE;

      DebugDumpHex(LOG_TRACE, "-ORG-", (uint8_t *)cTaskUart::m_uRxBufUart, m_u16RxBytesUart);
      wNewSize = 0;
      pRemoveCharBuf = ((cPacket *)g_stInst.pPacket)->pRemoveAddCharacters((uint8_t *)cTaskUart::m_uRxBufUart, m_u16RxBytesUart, &wNewSize);

      printk(LOG_DEBUG, F("g_u16RxBytes = %u\n"), wNewSize);
      DebugDumpHex(LOG_TRACE, "-RemoveChars-", (uint8_t *)pRemoveCharBuf, wNewSize);

      bRst = bGetRxPacketFromRxData((uint8_t *)pRemoveCharBuf, wNewSize, &bTsmCommand);
      m_u16RxBytesUart = 0;
//      if (pRemoveCharBuf)
//         free(pRemoveCharBuf);

      if (bRst != true)
      {
         m_u16RxPacketBufSize = 0;
         return uNowOp;
      }
      DebugDumpHex(LOG_TRACE, NULL, cTaskUart::m_u8RxPacketBuf, m_u16RxPacketBufSize);

      if (bTsmCommand)
      {
         u8NextOp = ((cPacket *)g_stInst.pPacket)->uProcessStringMsgPC((char *)cTaskUart::m_u8RxPacketBuf, m_u16RxPacketBufSize);
      }
      else
      {
         u8NextOp = ((cPacket *)g_stInst.pPacket)->uProcessPacketMsgPC(false, cTaskUart::m_u8RxPacketBuf, m_u16RxPacketBufSize);
         // printk(LOG_DEBUG, F("<PC>uNowOp = %d, u8NextOp=%u\n"), uNowOp, u8NextOp);
      }
      m_u16RxPacketBufSize = 0;
   }

   if (u8NextOp == OP_NONE)
      return uNowOp;
#endif	 
   return u8NextOp;
}

/*---------------------------------------------------------------------------------------
  function : Task_Uart()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
// void Task_Uart(void *pvParameters)
void cTaskUart::Task_Uart()
{
#if 0
   uint8_t u8NextOp;
   uint16_t u16NewSize;
   uint8_t *pRemoveCharBuf = NULL;
   bool bTsmCommand = false;
   bool bRst = false;
   cTaskUart::m_u16RxPacketBufSize = 0;

   while (1)
   {
		//((cTask *)g_stInst.pTask)->SemaGiveUartPc();
      uint8_t u8Rst = cTaskUart::nSerialEvent();

      if (u8Rst == RST_SUCCESS)
      {
         //DebugDumpHex(LOG_ERROR, "-ORG-", (uint8_t *)cTaskUart::m_uRxBufUart, 20);
         DebugDumpHex(LOG_TRACE, "-ORG-", (uint8_t *)cTaskUart::m_uRxBufUart, m_u16RxBytesUart);
         u16NewSize = 0;
         pRemoveCharBuf = ((cPacket *)g_stInst.pPacket)->pRemoveAddCharacters((uint8_t *)cTaskUart::m_uRxBufUart, m_u16RxBytesUart, &u16NewSize);

         printk(LOG_DEBUG, F("g_u16RxBytes = %u\n"), u16NewSize);
         //DebugDumpHex(LOG_ERROR, "-RemoveChars-", (uint8_t *)pRemoveCharBuf, 20);

         DebugDumpHex(LOG_TRACE, "-RemoveChars-", (uint8_t *)pRemoveCharBuf, u16NewSize);

         bRst = bGetRxPacketFromRxData((uint8_t *)pRemoveCharBuf, u16NewSize, &bTsmCommand);
         m_u16RxBytesUart = 0;
         //if (pRemoveCharBuf)
         //   free(pRemoveCharBuf);

         if (bRst != true)
         {
            m_u16RxPacketBufSize = 0;
            // return uNowOp;
            continue;
         }
         DebugDumpHex(LOG_DEBUG, NULL, cTaskUart::m_u8RxPacketBuf, m_u16RxPacketBufSize);

         if (bTsmCommand)
         {
            u8NextOp = ((cPacket *)g_stInst.pPacket)->uProcessStringMsgPC((char *)cTaskUart::m_u8RxPacketBuf, m_u16RxPacketBufSize);
         }
         else
         {
            u8NextOp = ((cPacket *)g_stInst.pPacket)->uProcessPacketMsgPC(false, cTaskUart::m_u8RxPacketBuf, m_u16RxPacketBufSize);
            // printk(LOG_DEBUG, F("<PC>uNowOp = %d, u8NextOp=%u\n"), uNowOp, u8NextOp);
         }
         m_u16RxPacketBufSize = 0;

         if (u8NextOp != OP_NONE)
         {
            printk(LOG_DEBUG, F("Task_Uart() u8NextOp = %u\n"), u8NextOp);
            ((cOpMode *)g_stInst.pOpMode)->u8OpProccess(u8NextOp);
            // g_stBleCfg.u8ResetCause = RESET_CONSOL_CMD;
            //((cTask *)g_stInst.pTask)->SendMessageQueue((char *)"m_QueueOp", TASK_ID_OP_1, m_QueueOp, -1, u8NextOp, 0, 0);
         }
      }
		//((cTask *)g_stInst.pTask)->SemaTakeUartPc((char*)"nSerialEvent()", 100);

      delay(3);
//      vTaskDelay(3 / portTICK_RATE_MS);
   }
   // Out side of loop now. Task needs to clean up and self terminate before returning
//   vTaskDelete(NULL);
#endif
}

