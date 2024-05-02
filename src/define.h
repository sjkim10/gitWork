/*****************************************************************************************
define.h
~~~~~~~~~~~~~~~~~~~~~~~~~
	@ Project : AirQuant
	@ Date : 2024-02-19
	@ Author : Piquant
*****************************************************************************************/

#ifndef _DEFINE_TEST_H_
#define _DEFINE_TEST_H_

#include <stdint.h>
#include <stdbool.h>
#include "sw_feature.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

typedef unsigned long   DWORD;
typedef unsigned short  WORD;
//typedef unsigned int  WORD;
typedef unsigned char   BYTE;

#define RST_SUCCESS        0
#define RST_ERROR          1
#define RST_FINISH         2
#define RST_SKIP           3
#define RST_NOTIFICATION   4
#define RST_POLLING        5
#define RST_CONNECT        6
#define RST_DISCONNECT     7
#define RST_PROCESSING     8
#define RST_COMPLETE       9
#define RST_CONTINUE       10
#define RST_EVT_DISCONNECTED  11
#define RST_WRONG_DATA     12
#define RST_NO_DATA        13 

typedef struct _stDouble2Bytes_t
{
   /* floating-point (ºÎµ¿¼Ò¼öÁ¡)
      - float  : in single (32-bit) presicion
      - double : in double (64-bit) presicion
   */
   float f;
   unsigned short w[sizeof(float) >> 1];
} __attribute__((packed)) stDouble2Bytes_t;


#define MAX_SENSOR      1 
#define MAX_SENSOR_ITEM 1


typedef struct _stResult_t
{
   uint8_t u8Result;
   stDouble2Bytes_t stData[MAX_SENSOR_ITEM];
} __attribute__((packed)) stResult_t;


#define SIZE_PACKET_ARGUMENT   3

#define F2(p) ((p))

#endif /* _DEFINE_TEST_H_ */

