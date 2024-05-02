/*****************************************************************************************
opmode.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _OPMODE_TEST_H_
#define _OPMODE_TEST_H_

#include "config.h"
#include "define.h"
#include "opmode_def.h"
#include "TemplateSingleTon.h"

/*****************************************************************************************
 *                                Extern & Definition
 *****************************************************************************************/
#define SR_ETH_ST_TAG    0xAA
#define SR_ETH_DY_TAG    0xBB
#define SR_WIFI_ST_TAG   0xCC
#define SR_WIFI_DY_TAG   0xDD
#define SR_BLE_TAG       0xEE
#define SR_ETC_TAG       0xFF



#define SOFT_RESET(a) (((a) == (0xAA)) || ((a) == (0xBB)) || ((a) == (0xCC)) || ((a) == (0xDD)) || ((a) == (0xEE)) || ((a) == (0xFF)))


//extern __NOINIT_ATTR uint8_t g_u8SwReset;
//extern __NOINIT_ATTR uint8_t g_u8SwResetCount;
extern uint8_t g_u8SwReset;
extern uint8_t g_u8SwResetCount;



#define SAVE_NVS      11
#define NO_SAVE_NVS   22

#define MAX_SW_RESET   2

extern uint32_t g_u32startGreenTm; //for Green LED



/*****************************************************************************************
 *                                 cOpMode  Class
 *****************************************************************************************/
class cOpMode : public TemplateSingleton<cOpMode>
{
public:
   cOpMode();
   virtual ~cOpMode();

	 uint8_t uDoInit(uint8_t uNowOP);
	 uint8_t uDoIdle(uint8_t uNowOP);
	 uint8_t uDoSleep(uint8_t uNowOP);
	 uint8_t uDoReset(uint8_t uNowOP);
	 uint8_t uDoBLE(uint8_t uNowOP);
	 uint8_t uDoConfig(uint8_t uNowOP);
	 uint8_t uDoCo2Cal(uint8_t uNowOP);
	 uint8_t uDoTest(uint8_t uNowOP);
	 
	 void cleanupPower();


public:
   static volatile uint8_t m_u8LoopStatus;
   static stDoOption_t m_stDoOption;

private:
    uint16_t m_u16ARGUMENT[SIZE_PACKET_ARGUMENT];
   bool m_bDirectlyCable;
   int8_t m_n8WarmmingUpCount;
   uint32_t m_u32OkPost;
   //bool m_bPostLED; gwlee 20220729 opmode -> task_natwork
   bool m_bNetPing;
   uint8_t m_u8IndexPostSensor;
};

#endif /* _OPMODE_TEST_H_ */


