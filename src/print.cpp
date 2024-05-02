/*****************************************************************************************
  print.c
 -------------------------------------
  @ Project : AirQuant
  @ Date : 2024-02-19
  @ Author : Piquant
*****************************************************************************************/
#include <Arduino.h>
#include <stdio.h>


#include "define.h"
#include "print.h"
#include "time_func.h"
#include "timer.h"
#include "task.h"
#include "utils.h"



/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
uint32_t g_un32LevelPrint = LOG_INFO;

void _printf_null(const uint32_t u32Level, const __FlashStringHelper *s, ...)
{
}

void _printf2_null(const uint32_t u32Level, const char* s, ...)
{
}


/*---------------------------------------------------------------------------------------
  function : _printk()
    @brief    : consol output
               _printk(F("input : %c\n"), receivedData);
    @param
    @return
----------------------------------------------------------------------------------------*/
void _printf(const uint32_t u32Level, const __FlashStringHelper *s, ...)
{
   if (u32Level > g_un32LevelPrint)
      return;

   va_list args;
   va_start(args, s);

   int n = vsnprintf(NULL, 0, (const char *)s, args);

   char *str = (char *)malloc(n + 1);
   vsprintf(str, (const char *)s, args);
   va_end(args);
   Serial.print(str);
   free(str);
}


void _printf2(const uint32_t u32Level, const char* s, ...)
{
   if (u32Level > g_un32LevelPrint)
      return;

   va_list args;
   va_start(args, s);

   int n = vsnprintf(NULL, 0, (const char *)s, args);

   char *str = (char *)malloc(n + 1);
   vsprintf(str, (const char *)s, args);
   va_end(args);
   Serial.print(str);
   free(str);
}

/*----------------------------------------------------------------------------------------
  function : setLogLevel()
    @brief    :
    @param
    @return
  -----------------------------------------------------------------------------------------*/
void setLogLevel(uint32_t u32Level)
{
   g_un32LevelPrint = u32Level;

   printk(LOG_TRACE, F("u32Level : 0x%08x\n"), u32Level);
}

/*----------------------------------------------------------------------------------------
  function : DebugDumpHex()
    @brief    : show the dump message
    @param
    @return
  -----------------------------------------------------------------------------------------*/
void DebugDumpHex(uint8_t uLevel, const char *sTitle, uint8_t *pBuf, uint16_t wCount, uint8_t uStep)
{
   if (uLevel > g_un32LevelPrint) return;

   if (sTitle != NULL)
   {
      printk(uLevel, F("%s (%u)\n"), sTitle, wCount);
   }
   for (int nIndex = 0; nIndex < wCount; nIndex++)
   {
      if (nIndex != 0 && nIndex % uStep == 0)
      {
         printk(uLevel, F("\n"));
      }
      printk(uLevel, F("%02x "), pBuf[nIndex]);
   }
   printk(uLevel, F("\n"));
}

/*----------------------------------------------------------------------------------------
  function : DebugDumpChar()
    @brief    : show the dump message
    @param
    @return
  -----------------------------------------------------------------------------------------*/
void DebugDumpChar(uint8_t uLevel, char *sTitle, uint8_t *pBuf, uint16_t wCount, uint8_t uStep)
{
   if (uLevel > g_un32LevelPrint) return;

   if (sTitle != NULL)
   {
      printk(uLevel, F("%s"), sTitle);
   }

   for (int nIndex = 0; nIndex < wCount; nIndex++)
   {
      if (nIndex != 0 && nIndex % uStep == 0)
      {
         printk(uLevel, F("\n"));
      }
      printk(uLevel, F("%c "), pBuf[nIndex]);
   }
   printk(uLevel, F("\n"));
}


/*---------------------------------------------------------------------------------------
  function : showAdvMsg()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void showAdvMsg(uint8_t u8Index, uint8_t* pData, uint16_t u16Size) 
{
  char sBuf[50];

	uint16_t u16devId = MAKEWORD(pData[0], pData[1]);
		
	switch (u8Index)
	{
	case SHOW_HEX_ALL :
		sprintf(sBuf, "Advertising Message(Device:%04x)", u16devId);
		DebugDumpHex(LOG_INFO, (const char*)sBuf, pData, u16Size, u16Size);
		break;
	case SHOW_HEX_DATA :
		sprintf(sBuf, "Adv Data(Device:%04x)", u16devId);		
		DebugDumpHex(LOG_INFO, (const char*)sBuf, pData, u16Size, u16Size);
		break;
	default:
		break;
	};
}

