/*****************************************************************************************
  main.cpp
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
#include <mbed.h>

#include "define.h"
#include "ble_beacon.h"
#include "opmode.h"
#include "config.h"
#include "init.h"
#include "uart.h"
#include "led2.h"
#include "system.h"
#include "timer.h"
#include "test.h"
#include "scd30_co2.h"
#include "co2_def.h"

#include "print.h"



/*--------------------------------------------------------------------------------------*/
#include "sleep.cpp"    // Sleep Module codes
/*--------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
using namespace mbed; 
using namespace rtos; //Thread
using namespace std::chrono_literals; //ThisThread::sleep_for(1s)

Thread Thread1;
Thread Thread2;


/*---------------------------------------------------------------------------------------
  function : funcThread()
    @brief    :
         BAT Register : Vcc -- 100k ---[ADC} -- 300K -- GND]
         ADC * (1/4096) x ref x 1/gain x (1/(3/4)) )
         ex) R + ADC + R, 10bit
         ADC * (1/1024) x ref(0.6) x gain(1/6) x 2 (R//R input)
          
    @param
    @return
----------------------------------------------------------------------------------------*/
void funcThread() 
{
	char sBuf[50];
	int nVal;
	static uint8_t u8PreStatus = 0;
	
  while(1)
	{ 
  	if(!g_bConnectedBle) continue;
    /* 12bit, */
		nVal = analogRead(BATTERY_LEVEL_PIN);
		g_stSensor.fBattery = (float)nVal * (1.0 / 4096.0) * 0.6 * 5.0 * (4.0 / 3.0);
    //nVolt = map(n8Val,0,4095,0,3300);
    cScd30Co2::u8GetStatusScd30_Co2();
//		sprintf((char*)sBuf, "%.1f,%.1f,%.1f(%u),%.1f(%u)", g_stSensor.fCo2Concentration,
//					g_stSensor.fTemperature, g_stSensor.fHumidity, g_stSensor.u8Status, g_stSensor.fBattery, nVal);
		sprintf((char*)sBuf, "%.1f, %u, %.1f(%u)", g_stSensor.fCo2Concentration,
					g_stSensor.u8Status, g_stSensor.fBattery, nVal);
		memset((void*)g_sSensorData, 0x0, MAX_SENSOR_DATA);
		strncpy((char*)g_sSensorData, sBuf, MAX_SENSOR_DATA);
		
		printk(LOG_DEBUG, F("%.1f,%.1f,%.1f(%u),%.1f(%u)\n"), g_stSensor.fCo2Concentration,
					g_stSensor.fTemperature, g_stSensor.fHumidity, g_stSensor.u8Status, g_stSensor.fBattery, nVal);

    ThisThread::sleep_for(1s);// Wait for 1 seconds
    //delay(10);
  }
}

/*---------------------------------------------------------------------------------------
  function : showPowerLed()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void showPowerLed()
{
#define LIMIT_SHOW_LED  1000
	static bool _bFirstPowrLed = true;
	static uint32_t _u32Pre = millis();

	while(_bFirstPowrLed)
	{
		uint32_t u32Now = millis();

		if((u32Now - _u32Pre) >= LIMIT_SHOW_LED) {
			cLed::set3Led(255, 255, 255);
	 		//_u32Pre = u32Now;
			_bFirstPowrLed = false;
			printk2(LOG_ALWAYS, "_bFirstPowrLed = %u\r", _bFirstPowrLed);
	 	}
		//printk2(LOG_ERROR, "%u = %u - %u, %u\r", (u32Now - _u32Pre), u32Now, _u32Pre, LIMIT_SHOW_LED);		
    //ThisThread::sleep_for(1s);
	}
}

/*---------------------------------------------------------------------------------------
  function : start_thread()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void start_thread() 
{
  Thread1.start(funcThread); 
	Thread2.start(showPowerLed);
}

/*---------------------------------------------------------------------------------------
  function : bInit()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
bool bInit() 
{
	static bool _bRst = true;

	openUart();

	if((_bRst == cInit::bInit())!=true) return _bRst;
#if 1	
	if((_bRst == ((cBleBeacon*)g_stInst.pBleBeacon)->bInitBle())!= true) return _bRst;

	cInit::showVersion();
#endif
	return _bRst;
}

/*---------------------------------------------------------------------------------------
  function : setup()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void setup() 
{
	if(bInit()!= true) 
	{
		while(1);
	}
	
#ifdef USE_SLEEP_CONTROL
	setTimerInterrupt();
#endif
	digitalWrite(LED_PWR, LOW);

#if 1
	digitalWrite(LED_PWR, LOW);
	digitalWrite(LED_BUILTIN, LOW);	
#endif

	start_thread();

	printk2(LOG_DEBUG, "setup(): end\n");
}


/*---------------------------------------------------------------------------------------
  function : loop()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void loop() 
{
#if 0
	if(!((cBleBeacon*)g_stInst.pBleBeacon)->bSendAdvertising())return;
	delay(100); 
	return;
#endif		

	static uint8_t _uNextOp = OP_INIT;
	static uint8_t _uNowOp = OP_INIT;
	cOpMode::m_u8LoopStatus = _uNextOp;
	cOpMode* pOp = ((cOpMode*)g_stInst.pOpMode);
	cUart* pUart = ((cUart*)g_stInst.pUart);	
	cTimer* pTimer = ((cTimer*)g_stInst.pTimer);		

//	cSystem::showPowerLed(3000);

#if 0
	_uNextOp = pUart->uProcessUart(_uNextOp);
return;
#endif

	switch (cOpMode::m_u8LoopStatus)
	{
	case OP_INIT:
		 _uNextOp = pOp->uDoInit(cOpMode::m_u8LoopStatus);
		 break;

	case OP_IDLE:
		 _uNextOp = pOp->uDoIdle(cOpMode::m_u8LoopStatus);
		 break;

	case OP_BLE:
		_uNextOp = pOp->uDoBLE(cOpMode::m_u8LoopStatus);
		break;

#ifdef USE_SLEEP_CONTROL	
	case OP_SLEEP:
		_uNextOp = uDoSleep(cOpMode::m_u8LoopStatus);
		break;
#endif

	case OP_RESET:
		_uNextOp = pOp->uDoReset(cOpMode::m_u8LoopStatus);
		break;

	case OP_CONFIG:
		_uNextOp = pOp->uDoConfig(cOpMode::m_u8LoopStatus);
		break;

	case OP_CO2:
		_uNextOp = pOp->uDoCo2Cal(cOpMode::m_u8LoopStatus);
		break;
		
	case OP_TEST://TSMTM
		_uNextOp = pOp->uDoTest(cOpMode::m_u8LoopStatus);
		break;
	
	default:
		_uNextOp = OP_INIT;
			
	}

	_uNextOp = pUart->uProcessUart(_uNextOp);

	cOpMode::m_u8LoopStatus = _uNextOp;


	((cTimer*)g_stInst.pTimer)->bResetWDT();

	//printk2(LOG_ERROR, "(2)_uNextOp = %u\n", _uNextOp);		
}




