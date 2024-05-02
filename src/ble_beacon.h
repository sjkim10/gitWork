/*****************************************************************************************
task_ble_beacon.h
~~~~~~~~~~~~~~~~~~~~~~~~~
	@ Project : AirQuant
	@ Date : 2024-02-19
	@ Author : Piquant
*****************************************************************************************/

#ifndef _TASK_BLE_BEACON_TEST_H_
#define _TASK_BLE_BEACON_TEST_H_

#include "define.h"
#include "TemplateSingleTon.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#if 0
#define MAX_SENSOR_DATA  17
#define MAX_SENSOR_NAME  3

typedef struct _stHead_st
{
	 uint8_t u8Leng;
	 uint8_t u8AdType;
} __attribute__((packed)) stHead_st;

typedef struct _stSet1_st
{
	 stHead_st stHead;
	 uint8_t u8Data1;
} __attribute__((packed)) stSet1_st;

typedef struct _stSet2_st
{
	 stHead_st stHead;
	 uint8_t u8DevId;
	 char sData2[MAX_SENSOR_DATA];
} __attribute__((packed)) stSet2_st;

typedef struct _stSet3_st
{
	 stHead_st stHead;
	 uint8_t u8Data3[MAX_SENSOR_NAME];
} __attribute__((packed)) stSet3_st;

typedef struct stUserData_st
{
	 stSet1_st s1;
	 stSet2_st s2;	 
	 stSet3_st s3;	 	 
} __attribute__((packed)) stUserData_st;

#else


#ifdef NO_CONNECT_ADVERTISING

#define MAX_MANU_ID      2
#define MAX_MANU_DATA    24

typedef struct _stManufacturingData_st
{
	 uint16_t u16DevId;
	 uint8_t u8Data[MAX_MANU_DATA];
} __attribute__((packed)) stManufacturingData_st;

//extern stManufacturingData_st g_stManufacturingData_st;

#else

#define MAX_SENSOR_DATA      26
extern volatile char g_sSensorData[MAX_SENSOR_DATA];
extern volatile bool g_bConnectedBle;

#endif

#endif

/*--------------------------------------------------------------------------------------*/
/*                Extern                                                                */
/*--------------------------------------------------------------------------------------*/
extern uint32_t g_u32StartBle;/* Time for BLE connection */



/*****************************************************************************************
 *                Ble beacon Class
 *****************************************************************************************/
class cBleBeacon : public TemplateSingleton<cBleBeacon>
{
public:
   cBleBeacon();
   virtual ~cBleBeacon();

   bool bInitBle(); 
   void exitBle();
#ifdef NO_CONNECT_ADVERTISING	 
	 bool bInitAdj_NoConnected();
	 bool bSendAdvertising_NoConnected();
#else
	 bool bInitAdj_Connected();	 
	 bool bSendAdvertising_Connected();
#endif	 
	 bool bSendAdvertising();

   bool bGetBleState();
   bool bGetBleMac(bool bShow);
   
   bool bGetRspData();
   bool bSetAdvData();
};

/*--------------------------------------------------------------------------------------*/
/*                Function                                                              */
/*--------------------------------------------------------------------------------------*/


#endif /* _TASK_BLE_BEACON_TEST_H_ */

