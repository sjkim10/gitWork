/*****************************************************************************************
  utils.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-02-24
  @ Author : Piquant
*****************************************************************************************/
#include <string.h>
#include "utils.h"
#include "print.h"


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

stParse_t cUtils::m_stPcParse;



/*---------------------------------------------------------------------------------------
  function 
----------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------
function : cUtils()
  * @brief   
  * @param   
  * @return
----------------------------------------------------------------------------------------*/
cUtils::cUtils()
{
}

/*---------------------------------------------------------------------------------------
function : ~cUtils()
  * @brief   
  * @param   
  * @return
----------------------------------------------------------------------------------------*/
cUtils::~cUtils()
{
}

/*---------------------------------------------------------------------------------------
function : bswap16()
  * @brief    : 0xaabb -> 0xbbaa
  * @param   
  * @return
----------------------------------------------------------------------------------------*/
inline WORD cUtils::bswap16(WORD a)
{
   return (a = ((a & 0x00FF) << 8) | ((a & 0xFF00) >> 8));
}

/*---------------------------------------------------------------------------------------
function : Bytes2Word()
  * @brief    : 2bytes --> WORD
  * @param   
  * @return
----------------------------------------------------------------------------------------*/
inline WORD cUtils::bytes2word(bool bReverse, BYTE *pByte)
{
   WORD wData, wTemp;
   BYTE nTag1, nTag2;
   if (bReverse)
   {
      nTag1 = 1;
      nTag2 = 0;
   }
   else
   {
      nTag1 = 0;
      nTag2 = 1;
   }
   wData = 0;
   wTemp = pByte[nTag1];
   wTemp <<= 8;
   wData |= wTemp;
   wData |= pByte[nTag2];
   return wData;
}



/*---------------------------------------------------------------------------------------
function : Word2Bytes()
  * @brief    : WORD --> 2bytes
  * @param   
  * @return
----------------------------------------------------------------------------------------*/
inline void cUtils::word2bytes(WORD wData, BYTE *b)
{
   *(WORD *)b = wData;
}

/*---------------------------------------------------------------------------------------
function : Bytes2Dword()
  * @brief    : bytes --> DWORD
  * @param   
  * @return
----------------------------------------------------------------------------------------*/
DWORD cUtils::bytes2dword(BYTE *pByte, BYTE nNum)
{
   DWORD dwData = 0, dwTemp;
   BYTE uShift[4] = {24, 16, 8, 0};
#ifdef REVERSE_ALIGN
   for (int i = nNum - 1; i >= 0; i--)
   {
      dwTemp = pByte[i];
      dwTemp <<= (uShift[4-nNum+i];
   }
#else
   for (int i = 0; i < nNum; i++)
   {
      dwTemp = pByte[i];
      dwTemp <<= (uShift[4 - nNum + i]);
   }
#endif
   return dwData;
}
/*---------------------------------------------------------------------------------------
function : Dword2Bytes()
  * @brief    : DWORD --> 4bytes
      DWORD dw = 0x01020304;
      char b[4];
      *(DWORD*)b = dw; // b is {0x04, 0x03, 0x02, 0x01}
  * @param   
  * @return
----------------------------------------------------------------------------------------*/
void cUtils::dword2bytes(DWORD dwData, BYTE *b)
{
#if 0
	BYTE *p = (BYTE *)&dwData;
	static BYTE array[4] = {*p+3, *p+2, *p+1, *p};

	return array;
#else
   *(DWORD *)b = dwData;
#endif
}

/*---------------------------------------------------------------------------------------
function : strtokenizer()
  * @brief    : 
  * @param   
  * @return
----------------------------------------------------------------------------------------*/
uint8_t cUtils::strtokenizer(char *pData, const char *pT)
{
	uint8_t u8Index = 0;

	for (uint8_t i = 0; i < NO_ARRAY_PARSE; i++)
	{
		memset(cUtils::m_stPcParse.d[i], 0x0, NO_SUB_ARRAY);
	}

	char *ptr = strtok(pData, pT); // strtok makes pointer which start from "indicator"

	while (ptr != NULL)
	{
		sprintf(cUtils::m_stPcParse.d[u8Index], "%s", ptr);
      printk(LOG_TRACE, F("cUtils::m_stPcParse.d[%d] : %s\n"), u8Index, cUtils::m_stPcParse.d[u8Index]);
		u8Index++;
		ptr = strtok(NULL, pT);
	}
	cUtils::m_stPcParse.noData = u8Index;

   for(uint8_t i = 0; i < 5; i++)
   {
      if(cUtils::m_stPcParse.d[u8Index - 1][i] == 0x0d)
      {
         cUtils::m_stPcParse.d[u8Index - 1][i] = 0;
         break;
      }
   }
	return u8Index;
}


/*---------------------------------------------------------------------------------------
function : average()
  * @brief    : 
  * @param   
  * @return
----------------------------------------------------------------------------------------*/
uint16_t cUtils::average (uint16_t x, uint16_t y)
{
  return (x&y)+((x^y)>>1);
}




