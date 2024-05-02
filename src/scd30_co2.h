/*****************************************************************************************
scd30_co2.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _SENSOR_SCD30_CO2_TEST_H_
#define _SENSOR_SCD30_CO2_TEST_H_

#include "define.h"
#include "TemplateSingleTon.h"


#ifdef USE_SCD30_CO2

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
typedef struct _stSensor_t
{
	float fCo2Concentration;
	float fTemperature;
	float fHumidity;
	uint8_t u8Status;
	float fBattery;
}stSensor_t;
extern stSensor_t g_stSensor;

/*****************************************************************************************
 *                              cScd30C02  Class
 *****************************************************************************************/
class cScd30Co2 : public TemplateSingleton<cScd30Co2>
{
public:
	cScd30Co2();
	virtual ~cScd30Co2();

	void initScd30_Co2();
	void turnOnOffScd30_Co2(bool bOn);
	bool calibrateScd30_Co2(uint8_t uAddr);
	static uint8_t u8GetStatusScd30_Co2();
	static void getCo2(float* pCo2, float* pTemp, float* pHum);	
	static void setForceRecalibration(uint16_t u16Baseline);	
	static void getForceRecalibration(uint16_t* pu16Baseline);
};
#endif



#endif /* _SENSOR_SCD30_CO2_TEST_H_ */

