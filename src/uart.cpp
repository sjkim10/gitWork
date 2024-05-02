/*****************************************************************************************
  uart.cpp
 -------------------------------------
	@ Project : AirQuant
	@ Date : 2024-02-19
	@ Author : Piquant
*****************************************************************************************/
#include <Arduino.h>

#include "uart.h"
#include "packet.h"
#include "utils.h"
#include "opmode.h"
#include "print.h"
#include "task.h"
#include "init.h"
#include "ble_beacon.h"



/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
bool g_bGotRxUart = false;  // whether the string is complete
uint8_t g_uRxBufUart1[MAX_RX_BUFFER_PC];
uint8_t g_u16RxSizeUart1;


/*---------------------------------------------------------------------------------------
  function : Write2PC()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
bool openUart()
{
	Serial.begin(115200);
	setLogLevel(LOG_TRACE);
//g_strRxBuf.reserve[256];//sjkim

	return true;
}

/*---------------------------------------------------------------------------------------
  function : Write2PC()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void Write2PC(uint8_t uCommand, uint8_t uRst, uint16_t *pArg, uint8_t *pData, uint16_t u16SizeDataOnly)
{
	stPacket_t *pstPacket;
	uint16_t u16TotalLen, u16NewTxSize;

	u16TotalLen = LEN_ONLY_BEFORE_DATA + u16SizeDataOnly + LEN_ONLY_AFTER_DATA;
	pstPacket = (stPacket_t *)malloc(sizeof(uint8_t) * u16TotalLen);

	if (u16SizeDataOnly > 0)
	{
	  memcpy(&pstPacket->_DATA[0], pData, u16SizeDataOnly);
	}

	u16NewTxSize = 0;
	uint8_t *pTxData = ((cPacket *)g_stInst.pPacket)->pMakeTxPacket(uCommand, uRst, pArg, (uint8_t *)pstPacket, u16TotalLen, &u16NewTxSize);
	if(pTxData == NULL)
	{
		free(pstPacket);	
		printk(LOG_ERROR, F("<Error> Write2PC()malloc error\n"));
		return;
	}


	// Serial.flush(true);
	Serial.write(pTxData, u16NewTxSize); /* CRC16, P-ETX, CR, LR */
	// printk(D_UART, LOG_DEBUG, F("Write2PC() <UART> [%u] cmd=%u, arg=%u,%u,%u, u16NewTxSize = %u\n"), cSocket::m_bConnectSocket, uCommand, pArg[0], pArg[1], pArg[2], u16NewTxSize);
	printk(LOG_DEBUG, F("Write2PC() <UART> cmd=%u, arg=%u,%u,%u, u16NewTxSize = %u\n"), uCommand, pArg[0], pArg[1], pArg[2], u16NewTxSize);

	DebugDumpHex(LOG_TRACE, "Write2PC Tx : ", (uint8_t *)pTxData, u16NewTxSize);

	free(pstPacket);
	free(pTxData);
}


/*---------------------------------------------------------------------------------------
  function : cUart()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cUart::cUart()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cUart()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cUart::~cUart()
{
}


/*---------------------------------------------------------------------------------------
  function : serialEvent()
    @brief    : 
				SerialEvent occurs whenever a new data comes in the hardware serial RX. This
				routine is run between each time loop() runs, so using delay inside loop can
				delay response. Multiple bytes of data may be available.
    @param
    @return
----------------------------------------------------------------------------------------*/
void serialEvent() 
{
  g_bGotRxUart = false;  
  while (Serial.available() > 0) {
    // get the new byte:
    char inChar = (char)Serial.read();
    {
      g_uRxBufUart1[g_u16RxSizeUart1++] = inChar;      
    }
    /* \r (0x0D) */
    if (inChar == '\r') 
		{     
			if(g_u16RxSizeUart1 > 1) g_bGotRxUart = true;
			else g_u16RxSizeUart1 = 0;
    }
  }
}

/*---------------------------------------------------------------------------------------
  function : uProcessUart()
    @brief :
    @param
    @return
----------------------------------------------------------------------------------------*/
uint8_t cUart::uProcessUart(uint8_t uNowOp)
{
	uint8_t uNextOp = uNowOp;
	
	serialEvent();

	
	if(g_bGotRxUart == 0) return uNextOp;
	
	if(g_bGotRxUart)
	{
		g_bGotRxUart = false;
		DebugDumpChar(LOG_ALWAYS, "<Rx>", g_uRxBufUart1, g_u16RxSizeUart1, g_u16RxSizeUart1);
	}

#if 0	
	printk(LOG_DEBUG, F("uProcessUart() g_u16RxSizeUart1=%u, %s)\r"), 
			g_u16RxSizeUart1, (char*)g_uRxBufUart1);
#endif

	uNextOp = ((cPacket*)g_stInst.pPacket)->uProcessStringMsgPC((char*)g_uRxBufUart1, g_u16RxSizeUart1);

	memset (g_uRxBufUart1, 0x0, MAX_RX_BUFFER_PC);
	g_u16RxSizeUart1 = 0;	

#if 0
	if(memcmp("RESET", (char*)sBuf, 5)== 0)
	{
		printf("== RESET ==\r");
		return OP_RESET;
	}
	else if(memcmp("MEASURE", (char*)sBuf, 6)== 0)
	{
		g_bConnectedBle = !g_bConnectedBle;
		printf("== MEASURE ==\r");
		return OP_BLE;
	}

	else if(memcmp("TEST", (char*)sBuf, 4)== 0)
	{
		printf("== TEST ==\r");
		return OP_TEST;		
	}
	else
	{
	
	}
#endif	

	printk(LOG_DEBUG, F("uProcessUart()-end :  uNextOp = %u\r"), uNextOp);
	

	return uNextOp;
}


