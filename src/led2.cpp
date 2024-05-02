/*****************************************************************************************
LED2.cpp
 -------------------------------------
	@ Project : AirQuant
	@ Date : 2024-02-23
	@ Author : Piquant
*****************************************************************************************/
#include <Arduino.h>
#include <SPI.h>

#include "led2.h"
#include "print.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
  function : cLed()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cLed::cLed()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cLed()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cLed::~cLed()
{
}

/*---------------------------------------------------------------------------------------
  function : initSpiMaster()
    @brief    : 
    @param
    @return
----------------------------------------------------------------------------------------*/
void cLed::initLed()
{
	printk(LOG_DEBUG, F("initLed()\n"));
	
#if 1
	// set the digital pin as output:
	pinMode(LED_PWR_PIN, OUTPUT);	
	digitalWrite(LED_PWR_PIN, LOW);  //Low Active

	//pinMode(LED_ADV_PIN, OUTPUT);
	//digitalWrite(LED_ADV_PIN, LOW);  

	pinMode(RED_PIN, OUTPUT);
	pinMode(GREEN_PIN, OUTPUT);
	pinMode(BLUE_PIN, OUTPUT);
	
	analogWrite(RED_PIN, LOW);
	analogWrite(GREEN_PIN, LOW);
	analogWrite(BLUE_PIN, LOW);	

	//pinMode(POWER_SWITCH_CO2, OUTPUT);	
	//digitalWrite(POWER_SWITCH_CO2, LOW);

  // the led on board
	pinMode(LED_PWR, OUTPUT);	
	pinMode(LED_BUILTIN, OUTPUT);

#else
  // the led on board
	pinMode(LED_PWR, OUTPUT);	
	pinMode(LED_BUILTIN, OUTPUT);

	pinMode(LEDR, OUTPUT);
	pinMode(LEDG, OUTPUT);
	pinMode(LEDB, OUTPUT);

	digitalWrite(LEDR, HIGH); //Off
	digitalWrite(LEDG, HIGH); //Off
	digitalWrite(LEDB, HIGH); //Off

	digitalWrite(LED_PWR, HIGH);  
	digitalWrite(LED_BUILTIN, LOW);  
#endif
}


/*---------------------------------------------------------------------------------------
  function : set3Led()
    @brief    : 
    	255, 255, 0    yellow
    	80,  0,   80   purple
    	0,   255, 255  aqua  
    @param
    @return
----------------------------------------------------------------------------------------*/
void cLed::set3Led(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue)
{
	printk(LOG_DEBUG, F("set3Led(%u, %u, %u)\n"), u8Red, u8Green, u8Blue);
	
	analogWrite(RED_PIN, u8Red);
	analogWrite(GREEN_PIN, u8Green);
	analogWrite(BLUE_PIN, u8Blue);	
}

/*---------------------------------------------------------------------------------------
  function : set3ResultLed()
    @brief    : 
    	255, 255, 0    yellow
    	80,  0,   80   purple
    	0,   255, 255  aqua  
    @param
    @return
----------------------------------------------------------------------------------------*/
void cLed::set3ResultLed(uint8_t u8IndexCo2, float fBat)
{
	static uint8_t _u8PreCo2 = 0;
	printk(LOG_DEBUG, F("set3ResultLed(%u, %u, %.1f)\n")
		               , u8IndexCo2, _u8PreCo2, fBat);
	if(fBat < 3.5) 
	{
		setPwrLed(OFF);//Off
		set3Led(255,255,255);	
	}
	else
	{
//		if(abs(u8IndexCo2 - _u8PreCo2) > 50)
		if(u8IndexCo2 != _u8PreCo2)
		{
///			setPwrLed(ON);//ON		
   		_u8PreCo2 = u8IndexCo2;  
			if(u8IndexCo2 <= 1/*VERY_GOOD_M*/) set3Led(255,255,0);	
			else if(u8IndexCo2 <= 2/*GOOD_M*/) set3Led(255,0, 255);		
			else  set3Led(0, 255, 255);		
		}
	}
}

/*---------------------------------------------------------------------------------------
  function : setAdvLed()
    @brief    : 
    @param
    @return
----------------------------------------------------------------------------------------*/
void cLed::setAdvLed(bool bOn)
{
	printk(LOG_TRACE, F("setAdvLed(%u)\n"), bOn);
	
	digitalWrite(LED_ADV_PIN, bOn);  
//	digitalWrite(LED_BUILTIN, bOn);  
}

/*---------------------------------------------------------------------------------------
  function : setOnboardLed()
    @brief    : LED mounted on board
    @param
    @return
----------------------------------------------------------------------------------------*/
void cLed::setOnboardLed(bool bOn)
{
	printk(LOG_DEBUG, F("setOnboardLed(%u)\n"), bOn);
	
	digitalWrite(LED_BUILTIN, bOn);  
}

/*---------------------------------------------------------------------------------------
  function : setPwrLed()
    @brief    : LED mounted on board
    @param
    @return
----------------------------------------------------------------------------------------*/
void cLed::setPwrLed(bool bOn)
{
	printk(LOG_DEBUG, F("setPwrLed(%u)\n"), bOn);
	if(bOn == ON) digitalWrite(LED_PWR_PIN, LOW);  
	else digitalWrite(LED_PWR_PIN, HIGH);  
}

/*---------------------------------------------------------------------------------------
  function : setColorLed()
    @brief    : LED mounted on board
    	255, 255, 0    yellow
    	80,  0,   80   purple
    	0,   255, 255  aqua  
    @param
    @return
----------------------------------------------------------------------------------------*/
void cLed::setColorLed(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue)
{
	printk(LOG_DEBUG, F("setColorLed(%u, %u, %u)\n"), u8Red, u8Green, u8Blue);
	
	analogWrite(LEDR, u8Red);
	analogWrite(LEDG, u8Green);
	analogWrite(LEDB, u8Blue);	
}

void cLed::led_onboard_test() 
{
	static int _nIndex = 0;
	if((_nIndex%12) == 0) setColorLed(1, 0, 0);
	else if((_nIndex%12) == 1) setColorLed(0, 1, 0);	
	else if((_nIndex%12) == 2) setColorLed(0, 0, 1);	
	else if((_nIndex%12) == 3) setColorLed(1, 1, 0);	
	else if((_nIndex%12) == 4) setColorLed(1, 0, 1);	
	else if((_nIndex%12) == 5) setColorLed(0, 1, 1);		
	else if((_nIndex%12) == 6) setColorLed(0, 0, 0);			
	else if((_nIndex%12) == 7) setColorLed(1, 1, 1);		
	else if((_nIndex%12) == 8) setPwrLed(ON);
	else if((_nIndex%12) == 9) setPwrLed(OFF);
	else if((_nIndex%12) == 10) setOnboardLed(HIGH);
	else if((_nIndex%12) == 11) setOnboardLed(LOW);	
	else _nIndex = -1;

	_nIndex++;

	delay(3000);
	
}	




