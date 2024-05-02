/*****************************************************************************************
  packet.cpp
 -------------------------------------
	@ Project : AirQuant
	@ Date : 2024-02-19
	@ Author : Piquant
*****************************************************************************************/

#include <Arduino.h>
#include "packet.h"

#include "init.h"
#include "uart.h"
#include "utils.h"
#include "opmode.h"
//#include "test.h"
#include "system.h"
#include "timer.h"
#include "task.h"
#include "print.h"
#include "gpio.h"
#include "scd30_co2.h"
#include "ble_beacon.h"


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

stPacketTail_t g_stPacketTail;
stCmd_t g_stCmd;
stReset_t g_stReset;

const unsigned char P_STX[4] = {"STM"};  /* 53 54 4D */
const unsigned char P_ETX[4] = {"MTS"};  /* 4D 54 53 */
const unsigned char P2_STX[4] = {"TSM"}; /* 54 53 4D */

const unsigned char CR_CHANGE[3] = {0xFF, 0xFE, 0xFD};
const unsigned char LF_CHANGE[3] = {0xFC, 0xFB, 0xFA};

const unsigned char CR = 0x0D; /* \r */
const unsigned char LF = 0x0A; /* \n */

const uint8_t uRESPONSE_OK[] = "OK";     /* 4F 4B */
const uint8_t uRESPONSE_FAIL[] = "FAIL"; /* 46 41 49 4C */



/*---------------------------------------------------------------------------------------
  function : cPacket()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cPacket::cPacket()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cWiFiDev()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cPacket::~cPacket()
{
}


/*---------------------------------------------------------------------------------------
function : uDoCommandPC()
@brief
@param
@return
----------------------------------------------------------------------------------------*/
uint8_t cPacket::uDoCommandPC(char *pCmd, stCmd_t *pstCmd, uint8_t *pOnlyData, uint16_t u16SizeOnlyData)
{
   bool b3Command = false;
   uint8_t sD[4] = {0x31, 0x32, 0x33, 0x34};

   printk(LOG_DEBUG, F("cmd : %c%c, pstCmd->uCmd = %u, (%u, %u, %u)\n"), pCmd[0], pCmd[1], pstCmd->u8Cmd, pstCmd->u16Item, pstCmd->u16Index, pstCmd->u16Index2);

   if ((strncmp(pCmd, PCCMD_ENTER_CFG, sizeof(PCCMD_ENTER_CFG) - 1) == 0) || (pstCmd->u8Cmd == CMD_ENTER_CFG))
   {
      printk(LOG_ALWAYS, F("[PC]PCCMD_ENTER_CFG ==\n"));
      Write2PC(CMD_ENTER_CFG, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));
      return OP_NONE;
   }	 
   else if ((strncmp(pCmd, PCCMD_GET_DATA, sizeof(PCCMD_GET_DATA) - 1) == 0) || (pstCmd->u8Cmd == CMD_GET_DATA))
   {
			g_bConnectedBle = !g_bConnectedBle;   
      printk(LOG_ALWAYS, F("[PC]CMD_GET_DATA(%u) ==\n"), g_bConnectedBle);
      Write2PC(pstCmd->u8Cmd, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));			
   }
   else if ((strncmp(pCmd, PCCMD_SET_DATA, sizeof(PCCMD_SET_DATA) - 1) == 0) || (pstCmd->u8Cmd == CMD_SET_DATA))
   {
      printk(LOG_ALWAYS, F("[PC]CMD_GET_DATA ==\n"));
      Write2PC(pstCmd->u8Cmd, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));			
   }
   else if ((strncmp(pCmd, PCCMD_FORCE_RECAL, sizeof(PCCMD_FORCE_RECAL) - 1) == 0) || (pstCmd->u8Cmd == CMD_FORCE_RECAL))
   {
      printk(LOG_ALWAYS, F("[PC]PCCMD_FORCE_RECAL(%u,%u,%u) ==\n"), g_stCmd.u16Item, g_stCmd.u16Index, g_stCmd.u16Index2);
			cScd30Co2::setForceRecalibration(g_stCmd.u16Item);
      Write2PC(pstCmd->u8Cmd, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));			
   }
   else if ((strncmp(pCmd, PCCMD_GET_CONFIG, sizeof(PCCMD_GET_CONFIG) - 1) == 0) || (pstCmd->u8Cmd == CMD_GET_CFG)) // TSMGI
   {
      printk(LOG_ALWAYS, F("[PC]PCCMD_GET_CONFIG ==\n"));
      Write2PC(pstCmd->u8Cmd, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));			
      g_stCmd.u16Item = ITEM_GET_CONFIG;
      g_stCmd.u8Cmd = CMD_GET_CFG;
      return OP_CONFIG;
   }
   else if ((strncmp(pCmd, PCCMD_SET_CONFIG, sizeof(PCCMD_SET_CONFIG) - 1) == 0) || (pstCmd->u8Cmd == CMD_SET_CFG))
   {
      printk(LOG_ALWAYS, F("[PC]PCCMD_SET_CONFIG ==\n"));   
      Write2PC(pstCmd->u8Cmd, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));			
		  return OP_CONFIG;
   }
   else if ((strncmp(pCmd, PCCMD_DO_RESET, sizeof(PCCMD_DO_RESET) - 1) == 0) || (pstCmd->u8Cmd == CMD_SET_RESET))
   {
      printk(LOG_ALWAYS, F("[PC]PCCMD_DO_RESET(%u, %u, %u)\n"), pstCmd->u16Item, pstCmd->u16Index, pstCmd->u16Index2);
      Write2PC(pstCmd->u8Cmd, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));
      pstCmd->u8Cmd = CMD_SET_RESET;
      return OP_RESET;
   }
   else if ((strncmp(pCmd, PCCMD_GET_STATUS, sizeof(PCCMD_GET_STATUS) - 1) == 0) || (pstCmd->u8Cmd == CMD_GET_STATUS)) // TSMGS
   {
      printk(LOG_ALWAYS, F("[PC]PCCMD_GET_STATUS ==\n"));
      g_stCmd.u16Item = ITEM_GET_STATUS;
      g_stCmd.u8Cmd = CMD_GET_STATUS;
      printk(LOG_ALWAYS, F("g_bConnectedBle = %u\n"), g_bConnectedBle);
      Write2PC(pstCmd->u8Cmd, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));			
      return OP_CONFIG;
   }
   else if ((strncmp(pCmd, PCCMD_GET_OP, sizeof(PCCMD_GET_OP) - 1) == 0) || (pstCmd->u8Cmd == CMD_GET_OP)) // TSMOP
   {
      uint8_t uLoopState = (uint8_t)cOpMode::m_u8LoopStatus;
      printk(LOG_ALWAYS, F("[PC]PCCMD_GET_OP : %d\n"), cOpMode::m_u8LoopStatus);
      Write2PC(CMD_GET_OP, RST_OK, m_u16ARGUMENT, (uint8_t *)&uLoopState, (uint16_t)sizeof(uint8_t));
   }

   else if ((strncmp(pCmd, PCCMD_RECONNECT_BLE, sizeof(PCCMD_RECONNECT_BLE) - 1) == 0) || (pstCmd->u8Cmd == CMD_RECONNECT_BLE)) // TSMRB
   {
      printk(LOG_ALWAYS, F("[PC]PCCMD_RECONNECT_BLE ==\n"));
      Write2PC(pstCmd->u8Cmd, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));			
   }
	 
   else if ((strncmp(pCmd, PCCMD_DEEP_SLEEP, sizeof(PCCMD_DEEP_SLEEP) - 1) == 0) || (pstCmd->u8Cmd == CMD_DEEP_SLEEP)) // TSMRB
   {
      printk(LOG_ALWAYS, F("[PC]PCCMD_DEEP_SLEEP ==\n"));
      ((cSystem *)g_stInst.pSystem)->gotoDeepSleep();
      Write2PC(CMD_DEEP_SLEEP, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));
      return OP_NONE;
   }

   else if ((strncmp(pCmd, PCCMD_CO2_CONTROL, sizeof(PCCMD_CO2_CONTROL) - 1) == 0) || (pstCmd->u8Cmd == CMD_CO2_CONTROL)) // TSMAA
	 {
		  printk(LOG_ALWAYS, F("[PC]PCCMD_CO2_CONTROL ==\n"));
		  Write2PC(CMD_CO2_CONTROL, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));
		  return OP_CO2;
	 }
	 
   else if ((strncmp(pCmd, PCCMD_TEST_MODE, sizeof(PCCMD_TEST_MODE) - 1) == 0) || (pstCmd->u8Cmd == CMD_DO_TESTMODE)) // TSMTM
   {
      printk(LOG_ALWAYS, F("[PC]PCCMD_TEST_MODE(%u,%u,%u) ==\n"));
      Write2PC(pstCmd->u8Cmd, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));			
      return OP_TEST;
   }
	 
   else
   {
      printk(LOG_ALWAYS, F("[PC]Else Command : %u\n"), pstCmd->u8Cmd);
      Write2PC(pstCmd->u8Cmd, RST_OK, m_u16ARGUMENT, (uint8_t *)uRESPONSE_OK, sizeof(uRESPONSE_OK));
   }

   return OP_NONE;
}


/*---------------------------------------------------------------------------------------
function : uProcessPacketMsgPC()
@brief : 
@param
@return
----------------------------------------------------------------------------------------*/
uint8_t cPacket::uProcessPacketMsgPC(bool bSoftAp, uint8_t *pRxPacket, uint16_t u16RxPacketBytes)
{
   uint16_t u16NewRxPacketBytes = 0;
   stPacket_t *pstPacket = (stPacket_t *)pRxPacket;
   uint8_t *pOnlyData = (uint8_t *)&pRxPacket[LEN_ONLY_BEFORE_DATA];
   uint16_t u16SizeOnlyData = u16RxPacketBytes - LEN_ONLY_BEFORE_DATA - LEN_ONLY_AFTER_DATA;
   uint8_t u8Rst;
   char sTemp[3] = {0};

   if (bVerifyPacketMsg(pRxPacket, u16RxPacketBytes, &u16NewRxPacketBytes) != true)
   {
      // Write2PC(pstPacket->_CMD, RST_ERR, m_u16ARGUMENT, (uint8_t *)uRESPONSE_FAIL, sizeof(uRESPONSE_FAIL));
      //printk(LOG_DEBUG, F("<Error>uProcessPacketMsgPC()bSoftAp=%u, Command (%u)\n"), bSoftAp, pstPacket->_CMD);
      printf("<Error>uProcessPacketMsgPC()bSoftAp=%u, Command (%u)\n", bSoftAp, pstPacket->_CMD);
      return OP_NONE;
   }
   m_u16ARGUMENT[0] = 0;
   m_u16ARGUMENT[1] = 0;
   m_u16ARGUMENT[2] = 0;
   g_stCmd.u8Cmd = pstPacket->_CMD;
   g_stCmd.u16Item = pstPacket->_ARG[0];
   g_stCmd.u16Index = pstPacket->_ARG[1];
   g_stCmd.u16Index2 = pstPacket->_ARG[2];

   g_stCmd.u8PreLoopStatus = cOpMode::m_u8LoopStatus;

   u8Rst = uDoCommandPC(sTemp, &g_stCmd, pOnlyData, u16SizeOnlyData);

   return u8Rst;
}


/*---------------------------------------------------------------------------------------
function : uProcessStringMsgPC()
@brief
@param
@return
----------------------------------------------------------------------------------------*/
uint8_t cPacket::uProcessStringMsgPC(char *pRxPacket, uint16_t u16RxPacketBytes)
{
   uint16_t uNo, uArg1, uArg2, uArg3;
   char *pData = pRxPacket + START_DATA_PC; /* TSM */
   // uint16_t u16NewRxPacketBytes = u16RxPacketBytes - START_DATA_PC;
   uint16_t u16NewRxPacketBytes = u16RxPacketBytes;
   char sTemp[MAX_RX_BUFFER_PC];

   printk(LOG_DEBUG, F("uProcessStringMsgPC(%s, %u)\n"), pRxPacket, u16RxPacketBytes);

//   if (strncmp(pData, PCCMD_SET_CONFIG, 2) != 0)
   {
      memcpy(sTemp, pData, u16NewRxPacketBytes);
      // DebugDump(NULL, (uint8_t *)sTemp, u16NewRxPacketBytes, 20);
      if ((uNo = cUtils::strtokenizer(sTemp, ",")) <= 0)
      {
         printk(LOG_ERROR, F("<Error> strtokenizer()\n"));

         return OP_NONE;
      }

      if (strlen(cUtils::m_stPcParse.d[0]) <= 1)
         return OP_NONE;
      if (strlen(cUtils::m_stPcParse.d[1]) <= 0)
         uArg1 = 0;
      else
         uArg1 = atoi(cUtils::m_stPcParse.d[1]);
      if (strlen(cUtils::m_stPcParse.d[2]) <= 0)
         uArg2 = 0;
      else
         uArg2 = atoi(cUtils::m_stPcParse.d[2]);
      if (strlen(cUtils::m_stPcParse.d[3]) <= 0)
         uArg3 = 0;
      else
         uArg3 = atoi(cUtils::m_stPcParse.d[3]);

      g_stCmd.u8Cmd = 0;
      g_stCmd.u16Item = uArg1;
      g_stCmd.u16Index = uArg2;
      g_stCmd.u16Index2 = uArg3;
      g_stCmd.u8PreLoopStatus = cOpMode::m_u8LoopStatus;

      printk(LOG_DEBUG, F("<%d> cUtils::m_stPcParse.d[0]=%s, arg1:%u, arg2:%u, arg3:%u, m_u8LoopStatus:%u\n")
       , uNo, cUtils::m_stPcParse.d[0], g_stCmd.u16Item, g_stCmd.u16Index, g_stCmd.u16Index2, g_stCmd.u8PreLoopStatus);
   };

   return uDoCommandPC(pData, &g_stCmd, (uint8_t *)&pData[3], u16RxPacketBytes - 3);
}

/*---------------------------------------------------------------------------------------
function : bVerifyPacketMsg()
@brief
@param
@return
----------------------------------------------------------------------------------------*/
bool cPacket::bVerifyPacketMsg(uint8_t *pRxPacket, uint16_t u16RxPacketBytes, uint16_t *u16NewRxPacketBytes)
{
   uint16_t u16RxSize = u16RxPacketBytes - 1; /* except CR */
   printk(LOG_TRACE, F("bVerifyPacketMsg(), u16RxPacketBytes = %d\n"), u16RxPacketBytes);

   *u16NewRxPacketBytes = (u16RxPacketBytes >= MAX_RX_BUFFER_PC) ? MAX_RX_BUFFER_PC : u16RxSize;
   stPacket_t *pHeader = (stPacket_t *)pRxPacket;

   if (*u16NewRxPacketBytes < u16RxSize)
   {
      printk(LOG_ERROR, F("<Error> bVerifyPacketMsg() u16NewRxPacketBytes = %u\n"), *u16NewRxPacketBytes);
      return false;
   }

   // *u16NewRxPacketBytes = pHeader->_LEN + Add Chars ( 0x0d -> 0x0d 0xff 0xfe 0xfd)
   if (*u16NewRxPacketBytes < pHeader->_LEN)
   // if (*u16NewRxPacketBytes != pHeader->_LEN)
   {
      printk(LOG_ERROR, F("\n<Error> bVerifyPacketMsg() Wrong length = %u, %u\n"), *u16NewRxPacketBytes, pHeader->_LEN);
      DebugDumpHex(LOG_TRACE, NULL, (uint8_t *)pRxPacket, *u16NewRxPacketBytes, 20);
      return false;
   }

   if (bCheckRxPakcet(pRxPacket, u16RxPacketBytes) != true)
   {
      return false;
   }
   return true;
}

/*---------------------------------------------------------------------------------------
function : bCheckRxPakcet()
  * @brief :
    Packet :
  * @param
  * @return
----------------------------------------------------------------------------------------*/
bool cPacket::bCheckRxPakcet(uint8_t *pRxPacket, uint16_t u16RxPacketBytes)
{
   stPacket_t *pHeader = (stPacket_t *)pRxPacket;
   stPacketTail_t *pstTail = (stPacketTail_t *)&pRxPacket[u16RxPacketBytes - LEN_ONLY_AFTER_DATA];

   uint8_t *pSTX = &pHeader->_STX[0];
   uint8_t *pETX = &pstTail->_ETX[0];

   DebugDumpHex(LOG_TRACE, "Head", (uint8_t *)pRxPacket, 20, 20);
   DebugDumpHex(LOG_TRACE, "Tail", (uint8_t *)&pRxPacket[u16RxPacketBytes - LEN_ONLY_AFTER_DATA], LEN_ONLY_AFTER_DATA, 20);

   if ((pSTX[0] != P_STX[0]) || (pSTX[1] != P_STX[1]) || (pSTX[2] != P_STX[2]) || (pETX[0] != P_ETX[0]) || (pETX[1] != P_ETX[1]) || (pETX[2] != P_ETX[2]))
   {
      printk(LOG_ERROR, F("Wrong STX/ETX (stx : 0x%x, 0x%x, 0x%x, etx : 0x%x, 0x%x, 0x%x)\n"), pSTX[0], pSTX[1], pSTX[2], pETX[0], pETX[1], pETX[2]);
      return false;
   }
   uint8_t u8Cmd = pHeader->_CMD;
   // if ((u8Cmd < CMD_SET_INIT) || (u8Cmd > CMD_MAX))
   if (u8Cmd >= CMD_MAX)
   {
      printk(LOG_ERROR, F("Wrong Commnad (cmd :0x%x)\n"), u8Cmd);
      return false;
   }
   uint16_t wSizeData = pHeader->_LEN; // MAKEWORD(pData[LLEN_BIT], pData[HLEN_BIT]);
   printk(LOG_TRACE, F("u16RxPacketBytes - LEN_ONLY_AFTER_DATA = %u\n"), u16RxPacketBytes - LEN_ONLY_AFTER_DATA);
   uint16_t wCRC16 = cPacket::wGenCrc16((uint8_t *)pRxPacket, u16RxPacketBytes - LEN_ONLY_AFTER_DATA);
   if (pstTail->_CRC16 != wCRC16)
   {
      printk(LOG_ERROR, F("Wrong CRC16 (rx:0x%x, calculate:0x%x), size = %u\n"), pstTail->_CRC16, wCRC16, wSizeData);
      return false;
   }
   if ((wSizeData < LEN_ONLY_BEFORE_DATA) || (wSizeData > SIZE_SENSOR_PACKET_DATA + LEN_ONLY_BEFORE_DATA))
   {
      printk(LOG_ERROR, F("Wrong Packet Length (length : 0x%x)\n"), wSizeData);
      return false;
   }
   return true;
}

/*---------------------------------------------------------------------------------------
function : wGenCrc16()
  * @brief    : make crc16
  * @param
  * @return
----------------------------------------------------------------------------------------*/
uint16_t cPacket::wGenCrc16(const uint8_t *pBuf, int nSize)
{
   int i = 0;
   uint16_t wCalcCrc16 = 0xffff;

   for (i = 0; i < nSize; i++)
   {
      wCalcCrc16 = ((wCalcCrc16 >> 8) | (wCalcCrc16 << 8)) & 0xffff;
      wCalcCrc16 ^= (pBuf[i] & 0xff);
      wCalcCrc16 ^= ((wCalcCrc16 & 0xff) >> 4);
      wCalcCrc16 ^= (wCalcCrc16 << 12) & 0xffff;
      wCalcCrc16 ^= ((wCalcCrc16 & 0xFF) << 5) & 0xffff;
   }
   wCalcCrc16 &= 0xffff;

   return wCalcCrc16;
}

/*---------------------------------------------------------------------------------------
=============================== ESP32 <--> SAMD21 ======================================
----------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
function : bMakeTxPacket()
  * @brief    : make Tx Packet
      (1) 0x0a --> 0xff 0xfe 0xfd 0x0a  --------> 0xff 0xfe 0xfd 0x0d 0x0a, In ESP32, 0x0a --> 0x0d 0x0d
      In ESP32, Data (0x0a) is always output as 0x0d 0x0a

      (2) 0x0d --> 0xfc 0xfb 0xfa 0x0d
  * @param
  * @return
----------------------------------------------------------------------------------------*/
uint8_t *cPacket::pMakeTxPacket(uint8_t uCommand, uint8_t uRst, uint16_t *pArg, uint8_t *pData, uint16_t wSizePacket, uint16_t *wNewSize)
{
   stPacket_t *pstPacket = (stPacket_t *)pData;
   pstPacket->_STX[0] = P_STX[0];
   pstPacket->_STX[1] = P_STX[1];
   pstPacket->_STX[2] = P_STX[2];

   pstPacket->_LEN = wSizePacket - 1; /* HEAD + Data + Tail (without CR) */
   pstPacket->_CMD = uCommand;
   pstPacket->_RST = uRst;

   pstPacket->_ARG[0] = pArg[0];
   pstPacket->_ARG[1] = pArg[1];
   pstPacket->_ARG[2] = pArg[2];
   //memcpy(pstPacket->_ARG, pArg, SIZE_PACKET_ARGUMENT * 2); /* 2byte */

   uint8_t *pDataTemp = (uint8_t *)pstPacket;
   uint16_t w = cPacket::wGenCrc16((uint8_t *)pstPacket, wSizePacket - LEN_ONLY_AFTER_DATA);

   pDataTemp[wSizePacket - 6] = LOBYTE(w);
   pDataTemp[wSizePacket - 5] = HIBYTE(w);
   pDataTemp[wSizePacket - 4] = P_ETX[0];
   pDataTemp[wSizePacket - 3] = P_ETX[1];
   pDataTemp[wSizePacket - 2] = P_ETX[2];
   pDataTemp[wSizePacket - 1] = CR;
   //   pDataTemp[wSizePacket - 1] = LF;

   uint8_t u8Found = 0;
   for (int i = 0; i < wSizePacket - 1; i++) /* except CR */
   {
      if ((pDataTemp[i] == CR /* \r, 0x0d */) || (pDataTemp[i] == LF /* \n, 0x0a */))
         u8Found++;
   }

   /*------------ ADD acddional charaters ------------------------*/
//   uint16_t u16NewTotalLen = wSizePacket + (u8Found * 4) + 1; /* include CR sapce */
   uint16_t u16NewTotalLen = wSizePacket + (u8Found * 4); /* include CR sapce */

   // printk(LOG_TRACE, F("pMakeTxPacket() : %u, %u, %u\n"), u16NewTotalLen, wSizePacket, u8Found);

   uint8_t *pNewTx = (uint8_t *)malloc(sizeof(uint8_t) * u16NewTotalLen);
	if(pNewTx == NULL) 
	{
   	printk(LOG_ERROR, F("pMakeTxPacket() : pMakeTxPacket(%u)\n"), sizeof(uint8_t) * u16NewTotalLen);
	   return NULL;
	}

   uint8_t *pTx = (uint8_t *)pDataTemp;
   uint16_t u16n = 0;
   for (int i = 0; i < wSizePacket - 1; i++) /* except CR */
   {
      /* (1) In ESP32, Data (0x0a) is always output as 0x0d 0x0a */
      if (pTx[i] == LF) /* 0x0a */
      {
         pNewTx[u16n++] = LF_CHANGE[0];
         pNewTx[u16n++] = LF_CHANGE[1];
         pNewTx[u16n++] = LF_CHANGE[2];
      }
      /* (2) When End-of-packet character is included in the data */
      if (pTx[i] == CR) /* 0x0d */
      {
         pNewTx[u16n++] = CR_CHANGE[0];
         pNewTx[u16n++] = CR_CHANGE[1];
         pNewTx[u16n++] = CR_CHANGE[2];
      }
      pNewTx[u16n++] = pTx[i];
   }
   pNewTx[u16n++] = CR;
//   pNewTx[u16n++] = 0;

   *wNewSize = u16n;

   return pNewTx;
}

/*---------------------------------------------------------------------------------------
function : pRemoveAddCharacters()
  * @brief    :
          (1) 0xff 0xfe 0xfd 0x0d 0x0a --> 0x0a
          (2) 0xfc 0xfb 0xfa 0x0d --> 0x0d
  * @param
  * @return
----------------------------------------------------------------------------------------*/
uint8_t *cPacket::pRemoveAddCharacters(uint8_t *pData, uint16_t wSizePacket, uint16_t *wNewSize)
{
   //   uint8_t *pNewTx = (uint8_t *)malloc(sizeof(uint8_t) * wSizePacket);
   // uint8_t *pNewTx = (uint8_t *)malloc(sizeof(uint8_t) * (wSizePacket + 1));
   static uint8_t _u8NewTx[1024];

   uint16_t i, u16n = 0;

   for (i = 0; i < wSizePacket - 4; i++) /* except 'M', 'T', 'S', and CR */
   {
      /* (1) In ESP32, Data (0x0a) is always output as 0x0d 0x0a */
      if ((pData[i] == LF_CHANGE[0]) && (pData[i + 1] == LF_CHANGE[1]) && (pData[i + 2] == LF_CHANGE[2]) && (pData[i + 3] == CR) && (pData[i + 4] == LF))
      {
         i += 4;
      }
      if ((pData[i] == LF_CHANGE[0]) && (pData[i + 1] == LF_CHANGE[1]) && (pData[i + 2] == LF_CHANGE[2]) && (pData[i + 3] == LF))
      {
         i += 3;
      }
      /* (2) In Data (0x0d) */
      if ((pData[i] == CR_CHANGE[0]) && (pData[i + 1] == CR_CHANGE[1]) && (pData[i + 2] == CR_CHANGE[2]) && (pData[i + 3] == CR))
      {
         i += 3;
      }
      _u8NewTx[u16n++] = pData[i];
   }
   _u8NewTx[u16n++] = pData[i++]; /* M */
   _u8NewTx[u16n++] = pData[i++]; /* T */
   _u8NewTx[u16n++] = pData[i++]; /* S */
   _u8NewTx[u16n++] = CR;

   *wNewSize = u16n;

   printk(LOG_DEBUG, F("pRemoveAddCharacters (org : %u, new = %u)\n"), wSizePacket, *wNewSize);

   return _u8NewTx;
}
