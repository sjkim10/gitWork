/*****************************************************************************************
  timer.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-02-24
  @ Author : Piquant
*****************************************************************************************/
#include <Arduino.h> // using Arduino development platform
//#include <esp_task_wdt.h>

#include "timer.h"
#include "system.h"
#include "init.h"
#include "gpio.h"
#include "print.h"


volatile bool g_bFlagWakeup;


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------
  function : cTimer()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cTimer::cTimer()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cTimer()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cTimer::~cTimer()
{
}

/*---------------------------------------------------------------------------------------
  function : setupHwTimer()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTimer::setupHwTimer(uint8_t uIndexTimer, uint32_t *u32RepeatTime)
{
}

/*---------------------------------------------------------------------------------------
  function : closeHwTimer()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTimer::closeHwTimer(uint8_t uIndexTimer)
{
}


/*---------------------------------------------------------------------------------------
  function : endHwTimer()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTimer::endHwTimer(uint8_t u8TimerIndex)
{
}

/*---------------------------------------------------------------------------------------
  function : startHwTimer()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTimer::startHwTimer(uint8_t u8TimerIndex)
{
}

/*---------------------------------------------------------------------------------------
  function : stopHwTimer()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTimer::stopHwTimer(uint8_t u8TimerIndex)
{
}

/*---------------------------------------------------------------------------------------
  function : bInitWDT()
    @brief    : Subscribe Idle Tasks to TWDT if they were not subscribed at startup
    @param
    @return
----------------------------------------------------------------------------------------*/
bool cTimer::bInitWDT(uint8_t u8Second)
{
	 //Configure WDT.
	 NRF_WDT->CONFIG				 = 0x01;		 // Configure WDT to run when CPU is asleep
// NRF_WDT->CRV 					 = 3932159;  // Timeout set to 120 seconds, timeout[s] = (CRV-1)/32768
	 NRF_WDT->CRV 					= u8Second * 32768 - 1;    // 120*32768 -1 
	 NRF_WDT->RREN					 = 0x01;		 // Enable the RR[0] reload register
	 NRF_WDT->TASKS_START 	 = 1; 			 // Start WDT 							

   return true;
}

/*---------------------------------------------------------------------------------------
  function : bExitWDT()
    @brief    : Delete and unsubscribe Users Tasks from Task Watchdog, then unsubscribe idle task
    @param
    @return
----------------------------------------------------------------------------------------*/
bool cTimer::bExitWDT()
{
	printk(LOG_ERROR, F("bExitWDT()\n"));

   return true;
}

/*---------------------------------------------------------------------------------------
  function : bResetWDT()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
bool cTimer::bResetWDT()
{
	// Reload the WDTs RR[0] reload register
	NRF_WDT->RR[0] = WDT_RR_RR_Reload;

	return true;
}
