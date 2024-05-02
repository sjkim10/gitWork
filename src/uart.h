/*****************************************************************************************
uart.h
~~~~~~~~~~~~~~~~~~~~~~~~~
	@ Project : AirQuant
	@ Date : 2024-02-19
	@ Author : Piquant
*****************************************************************************************/

#ifndef _UART_TEST_H_
#define _UART_TEST_H_


#include "define.h"
#include "TemplateSingleTon.h"


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
/* UART Buffer */
#define MAX_RX_BUFFER_PC 256

extern uint8_t g_uRxBufUart1[MAX_RX_BUFFER_PC];
extern uint8_t g_u16RxSizeUart1;

extern bool g_bGotRxUart;


/*****************************************************************************************
 *                                 cUart  Class
 *****************************************************************************************/
bool openUart();
bool bGetRxPacketFromRxData(uint8_t *pRxRawBuf, uint16_t u16RxRawSize, bool *pbTsmCommand);
void Write2PC(uint8_t uCommand, uint8_t uRst, uint16_t *pArg, uint8_t *pData, uint16_t u16SizeDataOnly);


/*****************************************************************************************
 *                              cUart  Class
 *****************************************************************************************/
class cUart : public TemplateSingleton<cUart>
{
public:
	cUart();
	virtual ~cUart();
	uint8_t uProcessUart(uint8_t uNowOp); 
};

   
#endif /* _UART_TEST_H_ */

