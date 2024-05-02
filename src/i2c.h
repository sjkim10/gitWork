/*****************************************************************************************
i2c.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _I2C_TEST_H_
#define _I2C_TEST_H_

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
 *                              cI2C  Class
 *****************************************************************************************/
class cI2c : public TemplateSingleton<cI2c>
{
public:
   cI2c();
   virtual ~cI2c();

   void scanI2cMaster();
   bool bProveI2C_Master(uint8_t uAddr);

   void setupI2cMaster();
	 void write_I2C(uint8_t u8SlaveAddr, uint8_t u8Data);
	 int nRead_I2C(uint8_t u8SlaveAddr, uint8_t u8Size);
	 void writeRead_I2C(uint8_t u8SlaveAddr, uint8_t* pTxData, uint8_t u16TxSize, uint8_t* pRxData, uint8_t u16RxSize);
	 static void RecoverI2cLockup(void);
};

#endif /* _I2C_TEST_H_ */
