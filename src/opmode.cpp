/*****************************************************************************************
  opmode.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-02-24
  @ Author : Piquant
*****************************************************************************************/
#include <Arduino.h>
#include <nrf_gpio.h>
#include <nrf_nvic.h>
#include <nrf_soc.h>
#include <nrf_wdt.h>
#include <pinDefinitions.h>

#include "opmode.h"
#include "opmode_def.h"
#include "timer.h"
#include "time_func.h"
#include "init.h"
#include "system.h"

#include "ble_beacon.h"

#include "system.h"
#include "utils.h"
#include "task.h"
#include "gpio.h"
#include "ble_beacon.h"
#include "packet.h"
#include "led2.h"
#include "packet.h"
#include "scd30_co2.h"
#include "test.h"

#include "print.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
uint32_t g_u32startGreenTm; //for Green LED


volatile uint8_t cOpMode::m_u8LoopStatus = OP_INIT;

/* The macro __NOINIT_ATTR can be used as attribute to place data into .noinit section.
   The values placed into this section will not be initialized at startup
   and should keep its value after software restart.
   0xAA : Software Reset
*/
//__NOINIT_ATTR 
uint8_t g_u8SwReset = 0;
//__NOINIT_ATTR 
uint8_t g_u8SwResetCount = 0;



/*---------------------------------------------------------------------------------------
  function : cOpMode()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cOpMode::cOpMode()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cOpMode()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cOpMode::~cOpMode()
{
}

/*---------------------------------------------------------------------------------------
  function : uDoInit()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
uint8_t cOpMode::uDoInit(uint8_t uNowOP)
{
	//printk(LOG_DEBUG, F("uDoInit()\n"));

	return OP_IDLE;
}

/*---------------------------------------------------------------------------------------
  function : uDoIdle()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
uint8_t cOpMode::uDoIdle(uint8_t uNowOP)
{
	static bool _bFirstDo = true;
	uint8_t u8NextOp = OP_BLE;
	//printk(LOG_ALWAYS, F("uDoIdle(), _bFirstDo=%u\n"), _bFirstDo);

	if (_bFirstDo == true)
	{
		_bFirstDo = false;
		printk(LOG_DEBUG, F("uDoIdle()\n"));
	}
	return u8NextOp;
}

/*---------------------------------------------------------------------------------------
  function : uDoReset()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
uint8_t cOpMode::uDoReset(uint8_t uNowOP)
{
	digitalWrite(LED_BUILTIN, HIGH);  
	//delay(500);
	printk2(LOG_ALWAYS, "uDoReset()\n");

	//BOOT_DOUBLE_TAP_DATA = DOUBLE_TAP_MAGIC;		// Start in bootloader mode after reset
	NVIC_SystemReset(); 							// Reset the microcontroller

	return uNowOP;
}

/*---------------------------------------------------------------------------------------
  function : uDoBLE()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
uint8_t cOpMode::uDoBLE(uint8_t uNowOP)
{
	 //printk2(LOG_ALWAYS, "uDoBLE()\n");

	 if(!((cBleBeacon*)g_stInst.pBleBeacon)->bSendAdvertising())return uNowOP;

#ifdef USE_SLEEP_CONTROL	 
   return OP_SLEEP;
#else
	return OP_IDLE;
#endif
}


/*---------------------------------------------------------------------------------------
  function : uDoConfig()
    @brief    :
    @param
    @return

----------------------------------------------------------------------------------------*/
uint8_t cOpMode::uDoConfig(uint8_t uNowOP)
{
	 printk2(LOG_ALWAYS, "uDoConfig()\n");

   return 1;
}

/*---------------------------------------------------------------------------------------
  function : uDoCo2Cal()
    @brief    :
    @param
    @return

----------------------------------------------------------------------------------------*/
uint8_t cOpMode::uDoCo2Cal(uint8_t uNowOP)
{
	 uint16_t u16Temp = 0;
	 printk2(LOG_ALWAYS, "uDoCo2Cal()\n");

   switch(g_stCmd.u16Item)
	 {
		 case SET_RECAL_SCD:
			 ((cScd30Co2*)g_stInst.pCo2)->setForceRecalibration(g_stCmd.u16Index);
			 printk2(LOG_ALWAYS, "setForceRecalibration(%u)\n", g_stCmd.u16Index);
			 break;
		 case GET_RECAL_SCD:
			 ((cScd30Co2*)g_stInst.pCo2)->getForceRecalibration(&u16Temp);
			 printk2(LOG_ALWAYS, "getForceRecalibration()u16Temp = %u\n", u16Temp);
			 break;
		 default:
				printk2(LOG_ERROR, "<Error>uDoCo2Cal(%u)\n", g_stCmd.u16Item);
				break;	
   }				

   return OP_NONE;
}

/*---------------------------------------------------------------------------------------
  function : uDoTest()
    @brief    :
    @param
    @return

----------------------------------------------------------------------------------------*/
uint8_t cOpMode::uDoTest(uint8_t uNowOP)
{
	printk2(LOG_ERROR, "uDoTest()\n");

	if(g_stCmd.u16Item == 0)
	{
		g_bConnectedBle = !g_bConnectedBle;

		printk2(LOG_ERROR, "uDoTest() g_bConnectedBle=%u, %u, %u,\r"
			,g_bConnectedBle, g_stCmd.u16Item, g_stCmd.u16Index);
	}
	
	else if(g_stCmd.u16Item == 1)
	{
		printk2(LOG_ERROR, "uDoTest(1) %u, %u, %u,\r"
			,g_stCmd.u16Item, g_stCmd.u16Index, g_stCmd.u16Index2);
			
		pinMode(LEDR, OUTPUT);//Active Low
		pinMode(LEDG, OUTPUT);//Active Low
		pinMode(LEDB, OUTPUT);//Active Low		
	
		if(g_stCmd.u16Index == 1)
		{
			digitalWrite(LEDR, HIGH); //Off
			digitalWrite(LEDG, HIGH); //Off
			digitalWrite(LEDB, HIGH); //Off
		}
		else
		{
			digitalWrite(LEDR, LOW);
			digitalWrite(LEDG, LOW);
			digitalWrite(LEDB, LOW);
		}
	}	
	else if(g_stCmd.u16Item == 2)
	{
		printk2(LOG_ERROR, "uDoTest(2) %u, %u, %u,\r"
			,g_stCmd.u16Item, g_stCmd.u16Index, g_stCmd.u16Index2);
#if 0
		pinMode(RED_PIN, OUTPUT);//Active Low
		pinMode(GREEN_PIN, OUTPUT);//Active Low
		pinMode(BLUE_PIN, OUTPUT);//Active Low		
#endif	
		if(g_stCmd.u16Index == 0)
		{
			if(g_stCmd.u16Index2 == 0) cLed::set3Led(0, 255, 255);
			else if(g_stCmd.u16Index2 == 1) cLed::set3Led(255, 0, 255);
			else if(g_stCmd.u16Index2 == 2) cLed::set3Led(255, 255, 0);
			else if(g_stCmd.u16Index2 == 3) cLed::set3Led(0, 0, 255);			
			else if(g_stCmd.u16Index2 == 4) cLed::set3Led(0, 255, 0);						
			else if(g_stCmd.u16Index2 == 5) cLed::set3Led(255, 0, 0);									
			else if(g_stCmd.u16Index2 == 6) cLed::set3Led(0, 0, 0);												
			else cLed::set3Led(255, 255, 255);
		}
		else
		{
		}
	}
	else if(g_stCmd.u16Item == 3)
	{
		printk2(LOG_ERROR, "uDoTest(3) %u, %u, %u,\r"
			,g_stCmd.u16Item, g_stCmd.u16Index, g_stCmd.u16Index2);

		if(g_stCmd.u16Index == 0)
		{ //OK
			if(g_stCmd.u16Index2 == 0) digitalWrite(LED_PWR_PIN, LOW);//Power LED On
			else digitalWrite(LED_PWR_PIN, HIGH);//Power LED Off
		}
		else
		{
		}
	}	
	else if(g_stCmd.u16Item == 4)
	{
		printk2(LOG_ERROR, "uDoTest(4) %u, %u, %u,\r"
			,g_stCmd.u16Item, g_stCmd.u16Index, g_stCmd.u16Index2);

		if(g_stCmd.u16Index == 0)
		{
			if(g_stCmd.u16Index2 == 0){
				digitalWrite(LED_PWR, LOW);//Power LED
				digitalWrite(LED_BUILTIN, LOW);
			}
			else {
				digitalWrite(LED_PWR, HIGH);//Power LED
				digitalWrite(LED_BUILTIN, HIGH);
			}
		}
		else
		{
		}
	}	
	else
	{
#ifdef USE_SCD30_CO2
	  ((cTest*)g_stInst.pTest)->readSCD30();
#endif
	}



  return 1;
}





