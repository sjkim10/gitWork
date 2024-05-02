/*****************************************************************************************
Print.h
~~~~~~~~~~~~~~~~~~~~~~~~~

@ Project : AirQuant
@ Date : 2024-02-19
@ Author : Piquant
*****************************************************************************************/

#ifndef _PRINT_TEST_H_
#define _PRINT_TEST_H_
#include <WString.h>

#include <stdio.h>
#include <stdarg.h>
#include "define.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

#ifdef DEBUG_MODE
#define printk _printf
#define printk2 _printf2

#else
#define printk _printf_null
#define printk2 _printf2_null

#endif

extern uint32_t g_un32LevelPrint;


/* Log Level */
enum
{
   LOG_ALWAYS = 0,  /* (printk(LOG_ALWAYS) means all logs are printed. */         
   LOG_NONE = 0,   
   LOG_FATAL,
   LOG_ERROR,
   LOG_WARN,
   LOG_INFO,
   LOG_DEBUG,
   LOG_TRACE,
   LOG_SET_LEVEL_ALWAYS, /* if (g_un32LevelPrint == LOG_ALWAYS) all logs are printed. */
};

enum
{
	SHOW_HEX_ALL = 0,
	SHOW_HEX_DATA,
};



/*--------------------------------------------------------------------------------------*/
/*                Function                                                              */
/*--------------------------------------------------------------------------------------*/
extern void setLogLevel(uint32_t u32Level);
extern void DebugDumpHex(uint8_t uLevel, const char *sTitle, uint8_t *pBuf, uint16_t wCount, uint8_t uStep = 20);
extern void DebugDumpChar(uint8_t uLevel, char *sTitle, uint8_t *pBuf, uint16_t wCount, uint8_t uStep);
extern void showAdvMsg(uint8_t u8Index, uint8_t* pData, uint16_t u16Size);


extern void _printf2(const uint32_t u32Level, const char* s, ...);
extern void _printf2_null(const uint32_t u32Level, const char* s, ...);

extern void _printf(const uint32_t u32Level, const __FlashStringHelper *s, ...);
extern void _printf_null(const uint32_t u32Level, const __FlashStringHelper *s, ...);

#endif /* _PRINT_TEST_H_ */
