/*****************************************************************************************
  sesnor_co2.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-02-24
  @ Author : Piquant
*****************************************************************************************/
#include <Arduino.h>
#include <SensirionI2cScd30.h>
#include <Wire.h>
#include "scd30_co2.h"
#include "gpio.h"
#include "i2c.h"
#include "co2_def.h"

#include "print.h"


#ifdef USE_SCD30_CO2
/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
SensirionI2cScd30 sensor;

stSensor_t g_stSensor;

/*---------------------------------------------------------------------------------------
  function : cScd30Co2()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cScd30Co2::cScd30Co2()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cScd30Co2()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cScd30Co2::~cScd30Co2()
{
}

/*---------------------------------------------------------------------------------------
  function : initScd30_Co2()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cScd30Co2::initScd30_Co2()
{
	char errorMessage[128];
	int16_t error2;	
	uint8_t major = 0;
	uint8_t minor = 0;

	sensor.begin(Wire, SCD30_I2C_ADDR_61);
	sensor.stopPeriodicMeasurement();
	sensor.softReset();
	delay(2000);
	if((error2 = sensor.readFirmwareVersion(major, minor)) != NO_ERROR) 
	{
		printk(LOG_ERROR, F("<Error>sensor.readFirmwareVersion()\n"));	
		errorToString(error2, errorMessage, sizeof errorMessage);
		printk(LOG_ERROR, F("err : %s\n"), errorMessage);	
		return;
	}
#if 1	
	error2 = sensor.startPeriodicMeasurement(0);
	if (error2 != NO_ERROR) 
	{
		printk(LOG_ERROR, F("<Error>sensor.startPeriodicMeasurement()\n"));		
		errorToString(error2, errorMessage, sizeof errorMessage);
		printk(LOG_ERROR, F("err : %s\n"), errorMessage); 
		return;
	}
#endif	

//	sensor.forceRecalibration(1100);
}


/*---------------------------------------------------------------------------------------
  function : turnOnOffScd30_Co2()
    @brief    : 
    @param
    @return
----------------------------------------------------------------------------------------*/
void cScd30Co2::turnOnOffScd30_Co2(bool bOn)
{
   printk(LOG_DEBUG, F("turnOnOffScd30_Co2(%d)\n"), bOn);

	 digitalWrite(POWER_SWITCH_CO2, bOn);  
}

/*---------------------------------------------------------------------------------------
  function : calibrateScd30_Co2()
    @brief    : prove
    @param
    @return
----------------------------------------------------------------------------------------*/
bool cScd30Co2::calibrateScd30_Co2(uint8_t uAddr)
{
   printk(LOG_DEBUG, F("calibrateScd30_Co2()\n"));


   return false;
}

/*---------------------------------------------------------------------------------------
  function : u8GetStatusScd30_Co2()
    @brief    :
    	CO2(ppm) < 700 ppm --> Very good
    	CO2(ppm) < 1000 ppm --> good    	
    	CO2(ppm) < 2000 ppm --> bad
    	CO2(ppm) ≥ 2000 ppm --> Very bad
    @param
    @return
----------------------------------------------------------------------------------------*/
#define LIMIT_VAL   30.0
#define SAME_STATES 2

#define BAD_LIMIT_CO2  2500//3000
#define GOOD_LIMIT_CO2 1500//2500
#define VERY_GOOD_LIMIT_CO2 700//1500
uint8_t cScd30Co2::u8GetStatusScd30_Co2()
{
	static float _fOldStatus = 0;
	static uint8_t _u8Status = 0;
	static uint8_t _u8VeryGood = 0;
	static uint8_t _u8Good = 0;
	static uint8_t _u8Bad = 0;	
	char sBuf[50];
	float fCo2 = 0;
	float fT = 0;
	float fH = 0;

	printk(LOG_TRACE, F("u8GetStatusScd30_Co2()\n"));

	cScd30Co2::getCo2(&fCo2, &fT, &fH);

	g_stSensor.fCo2Concentration = fCo2;
	g_stSensor.fTemperature = fT;
	g_stSensor.fHumidity = fH;
	
	if(fCo2 < 300) _u8Status = WARM_UP_M;
	else 
	{
		if(fabs(fCo2 - _fOldStatus) > LIMIT_VAL)
		{
			_u8VeryGood = 0;
			_u8Good = 0;
			_u8Bad = 0;
			if(fCo2 < VERY_GOOD_LIMIT_CO2) _u8Status = VERY_GOOD_M;
			else if(fCo2 < GOOD_LIMIT_CO2) _u8Status = GOOD_M;
			else _u8Status = BAD_M;
//			else if(fCo2 < BAT_LIMIT_CO2) _u8Status = BAD_M;
//			else _u8Status = VERY_BAD;
		}
		else
		{
			if(fCo2 < VERY_GOOD_LIMIT_CO2) _u8VeryGood++;
			else if(fCo2 < GOOD_LIMIT_CO2) _u8Good++;
			else _u8Bad++;
			if(_u8VeryGood > SAME_STATES) _u8Status = VERY_GOOD_M;
			else if(_u8Good > SAME_STATES) _u8Status = GOOD_M;
			else if(_u8Bad > SAME_STATES) _u8Status = BAD_M;
			else;
			if((_u8VeryGood > SAME_STATES) || (_u8Good > SAME_STATES) || (_u8Bad > SAME_STATES))
			{
				_u8VeryGood = 0;
				_u8Good = 0;
				_u8Bad = 0;
			}
		}			
		_fOldStatus = fCo2;
	}
	g_stSensor.u8Status = _u8Status;

	printk(LOG_TRACE, F("u8GetStatusScd30_Co2(%u)end\n"), g_stSensor.u8Status);	
	
	return _u8Status;
}


/*---------------------------------------------------------------------------------------
  function : getCo2()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cScd30Co2::getCo2(float* pCo2, float* pTemp, float* pHum)
{
  printk(LOG_TRACE, F("getCo2()\n"));

	char errorMessage[128];
	int16_t error2;	
	float co2Concentration = 0.0;
	float temperature = 0.0;
	float humidity = 0.0;

	cI2c::RecoverI2cLockup();//sjkim

	error2 = sensor.blockingReadMeasurementData(*pCo2, *pTemp, *pHum);
	if (error2 != NO_ERROR) {
		printk(LOG_DEBUG, F("<Eror>sensor.blockingReadMeasurementData()\n"));
		errorToString(error2, errorMessage, sizeof errorMessage);
		printk(LOG_DEBUG, F("err : %s\n"), errorMessage);
		return;
	}
//	printk(LOG_DEBUG, F("CO2=%f, T=%f, H=%f\n"), *pCo2, *pTemp, *pHum);
}	

/*---------------------------------------------------------------------------------------
  function : setForceRecalibration()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cScd30Co2::setForceRecalibration(uint16_t u16Baseline)
{
  printk(LOG_ALWAYS, F("getForceRecalibration(%u)\n"), u16Baseline);
	sensor.forceRecalibration(u16Baseline);	
}

/*---------------------------------------------------------------------------------------
  function : getForceRecalibration()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cScd30Co2::getForceRecalibration(uint16_t* pu16Baseline)
{
	uint16_t u16Baseline = 0;

	sensor.getForceRecalibrationStatus(u16Baseline);	
	*pu16Baseline = u16Baseline;
  printk(LOG_ALWAYS, F("getForceRecalibration(%u)\n"), *pu16Baseline);	
}

#endif

