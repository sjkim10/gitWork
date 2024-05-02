/*****************************************************************************************
  sleep.cpp
 -------------------------------------
  @ Project : AirQuant
  @ Date : 2024-02-23
  @ Author : Piquant
*****************************************************************************************/
#include <ArduinoBLE.h>
#include <nrf_gpio.h>
#include <nrf_nvic.h>
#include <nrf_soc.h>
#include <pinDefinitions.h>

#include "define.h"
#include "print.h"


#ifdef USE_SLEEP_CONTROL

// To be included only in main(), xxx.ino with setup() 
// to avoid `Multiple Definitions` Linker Error
#include "NRF52_MBED_TimerInterrupt.h"
#include "NRF52_MBED_ISR_Timer.h"


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
#define HW_TIMER_INTERVAL_MS  				1



// Init NRF52 timer NRF_TIMER3
NRF52_MBED_Timer ITimer(NRF_TIMER_3);
// Each NRF52_MBED_ISRTimer can service 16 different ISR-based timers
NRF52_MBED_ISRTimer ISR_Timer;


/*---------------------------------------------------------------------------------------
  function : doWakeupTimerInt()
    @brief    : In NRF52, avoid doing something fancy in ISR, 
                for example complex Serial.print with String() argument
		            otherwise weu can get this run-time error / crash
    @param
    @return
----------------------------------------------------------------------------------------*/
void doWakeupTimerInt()
{
	g_bFlagWakeup = true;

  digitalWrite(LED_PWR, HIGH);		
	digitalWrite(POWER_SWITCH_CO2, HIGH);
	/* toggle */
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}


/*---------------------------------------------------------------------------------------
  function : TimerHandler()
    @brief    : 
    @param
    @return
----------------------------------------------------------------------------------------*/
void TimerHandler()
{
  ISR_Timer.run();
}

/*---------------------------------------------------------------------------------------
  function : setTimerInterrupt()
    @brief    : 
    @param
    @return
----------------------------------------------------------------------------------------*/
void setTimerInterrupt()
{
	// Interval in microsecs
	if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
	{
		Serial.print(F("Starting ITimer OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer. Select another freq. or timer"));
}

/*---------------------------------------------------------------------------------------
	function : TimerHandler()
		@brief		: 
		@param
		@return
----------------------------------------------------------------------------------------*/
void setTimeout()
{
	ISR_Timer.setTimeout(WAKEUP_TIME, doWakeupTimerInt);
}

/*---------------------------------------------------------------------------------------
  function : cleanupPower()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cleanupPower()
{
	printk2(LOG_ERROR, "cleanupPower\n");

#if 0
	// LED off
	digitalWrite(LED_BUILTIN, LOW);
	digitalWrite(LED_PWR, LOW);
	digitalWrite(LEDR, HIGH); //Off
	digitalWrite(LEDG, HIGH); //Off
	digitalWrite(LEDB, HIGH); //Off
	digitalWrite(POWER_SWITCH_CO2, LOW);
#else
	digitalWrite(LED_PWR_PIN, HIGH); //Low Active
	digitalWrite(POWER_SWITCH_CO2, LOW);


#endif
	printk2(LOG_ERROR, "LED Off\n"); 	

}

/*---------------------------------------------------------------------------------------
  function : uDoSleep()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
uint8_t uDoSleep(uint8_t uNowOP)
{
	static bool _bFirstDo = true;
	uint8_t u8NextOp = OP_NONE;

	if (_bFirstDo == true)
	{
		_bFirstDo = false;
		printk(LOG_DEBUG, F("uDoSleep()\n"));

		cleanupPower();

		//((cTimer*)g_stInst.pTimer)->setTimeout();
		ISR_Timer.setTimeout(WAKEUP_TIME, doWakeupTimerInt);

#if 0
	  // low power comparator setup to wake-up when the capacitor is discharged to 1/16 of VIN
	  NRF_LPCOMP->PSEL=1; //select AIN3 = PIN_A1 = P0.05
	  NRF_LPCOMP->REFSEL=8; // VDD * 1/16 selected as reference
	  NRF_LPCOMP->ENABLE=1;
	  NRF_LPCOMP->TASKS_START=1;
	  while(!NRF_LPCOMP->EVENTS_READY){}

	  NRF_POWER->SYSTEMOFF = 1;
#else

		// disable RTC1 interrupts
		sd_nvic_DisableIRQ(RTC1_IRQn);

		// sleep until wake-up flag has been set by timer interrupt-service routine
		while (!g_bFlagWakeup)
		{
			sd_app_evt_wait();
		}
		
		// enable RTC1 interrupts
		sd_nvic_EnableIRQ(RTC1_IRQn);

		printk2(LOG_ERROR, "uDoSleep() : end\n");		

#endif		
	}
	
	return u8NextOp;
}

#endif //#ifdef USE_SLEEP_CONTROL

