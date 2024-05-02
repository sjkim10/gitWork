/*****************************************************************************************
config.h
~~~~~~~~~~~~~~~~~~~~~~~~~
	@ Project : AirQuant
	@ Date : 2024-02-19
	@ Author : Piquant
*****************************************************************************************/

#ifndef _CONFIG_TEST_H_
#define _CONFIG_TEST_H_

#include "define.h"



/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#define DEVICE_ID   0x1234
#define DEVICE_NAME "PQBLE"

#define SIZE_HOSTNAME      4
#define SIZE_SN            16
#define SIZE_MACADDR       6

/*--------------------------------------------------------------------------------------*/
/*                Extern                                                                */
/*--------------------------------------------------------------------------------------*/
#define BLE_DEV_NAME "PQBLE"

extern const char* UDID;
extern const char* SERIAL_NO;


typedef struct _stDevVersion_t
{
	char sBuildDate[12];
	uint8_t u8Major;
	uint8_t u8Minor;
	uint8_t u8Minor2;	
} __attribute__((packed)) stDevVersion_t;


typedef struct _stDeviceInfo_t
{
	stDevVersion_t stVer[2];
	char sUDID[SIZE_HOSTNAME];
	char sSERIAL_NO[SIZE_SN];
	uint8_t sMac[SIZE_MACADDR];
} __attribute__((packed)) stDeviceInfo_t;


extern stDevVersion_t g_stVersion;
extern stDeviceInfo_t g_stDeviceInfo;

/*--------------------------------------------------------------------------------------*/
/*                Function                                                              */
/*--------------------------------------------------------------------------------------*/




#endif /* _CONFIG_TEST_H_ */

