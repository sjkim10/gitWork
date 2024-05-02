/*****************************************************************************************
test.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _MY_TEST_TEST_H_
#define _MY_TEST_TEST_H_

#include "define.h"
#include "TemplateSingleTon.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#define I2C_SDA  PIN_WIRE_SDA
#define I2C_SCL  PIN_WIRE_SCL

#define	STD_CLK_I2C  100000
#define	FAST_CLK_I2C 400000
#define FAST2_CLK_I2C 1000000
#define HI_FAST_CLK_I2C 3400000


void requestEvent_slave();

/*****************************************************************************************
                        cTest  Class
 *****************************************************************************************/
class cTest : public TemplateSingleton<cTest>
{
public:
   cTest();
   virtual ~cTest();

   void readSCD30();
};

#endif /* _MY_TEST_TEST_H_ */
