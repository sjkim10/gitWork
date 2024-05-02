/*****************************************************************************************
  test.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-02-24
  @ Author : Piquant
*****************************************************************************************/
#include <Arduino.h>
#include <Wire.h>

#include "i2c.h"
#include "gpio.h"
#include "scd30_co2.h"
#include "init.h"
#include "test.h"
#include "print.h"




/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------------------
  function : cTest()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cTest::cTest()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cI2c()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cTest::~cTest()
{
}

/*---------------------------------------------------------------------------------------
  function : readSCD30()
    @brief   :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTest::readSCD30()
{
	char sBuf[128];
	uint8_t u8P = ((cScd30Co2*)g_stInst.pCo2)->u8GetStatusScd30_Co2();
	sprintf((char*)sBuf, "%.1f,%.1f,%.1f,%u", g_stSensor.fCo2Concentration,
	g_stSensor.fTemperature, g_stSensor.fHumidity, u8P);
	printk(LOG_ERROR, F("scd30 : %s\n"), sBuf);	
	return;

}



