/*****************************************************************************************
rx9_co2.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _RX9_CO2_TEST_H_
#define _RX9_CO2_TEST_H_

#include "define.h"
#include "TemplateSingleTon.h"
#include "gpio.h"


#ifdef USE_RX9_CO2

//https://colorscripter.com/s/Jnrf03K

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#if 0
#define I2C_SDA_PIN  PIN_WIRE_SDA
#define I2C_SCL  PIN_WIRE_SCL
#endif



/*****************************************************************************************
 *                              cRx9Co2  Class
 *****************************************************************************************/
class cRx9Co2 : public TemplateSingleton<cRx9Co2>
{
public:
   cRx9Co2();
   virtual ~cRx9Co2();

   void init_Rx9_Co2();
   bool calibrate_Rx9_Co2(uint8_t uAddr);
	 float fGet_Rx9_Co2();

	float Show_Rx9_Co2();
	uint8_t u8GetStatus_Rx9_Co2();
	unsigned int unGet_Rx9_Co2();
	void turnOnOff_Rx9_Co2(bool bOn);
};

#endif
#endif /* _RX9_CO2_TEST_H_ */

