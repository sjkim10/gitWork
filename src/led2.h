/*****************************************************************************************
led2.h
~~~~~~~~~~~~~~~~~~~~~~~~~
	@ Project : AirQuant
	@ Date : 2024-02-23
	@ Author : Piquant
*****************************************************************************************/

#ifndef _LED_TEST_H_
#define _LED_TEST_H_

#include "define.h"
#include "TemplateSingleTon.h"
#include "gpio.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#if 0
#ifdef USE_3LED
#define RED_PIN   D2  //Bad
#define GREEN_PIN D3  //Good
#define BLUE_PIN  D5  //Not Bad 
#endif

#define LED_ADV_PIN		D9 //Tx Advertising Data (No PWM)
#endif

/*****************************************************************************************
 *                              cLED  Class
 *****************************************************************************************/
class cLed : public TemplateSingleton<cLed>
{
public:
	cLed();
	virtual ~cLed();

	void initLed();
	static void set3Led(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue);
	static void set3ResultLed(uint8_t u8IndexCo2, float fBat);
	void setAdvLed(bool bOn);
	void setOnboardLed(bool bOn);
	static void setPwrLed(bool bOn);
	void setColorLed(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue);

	void led_onboard_test();
};

#endif /* _LED_TEST_H_ */

