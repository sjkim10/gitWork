/*****************************************************************************************
  i2c.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-02-24
  @ Author : Piquant
*****************************************************************************************/
#include <Arduino.h>
#include <Wire.h>

#include "i2c.h"
#include "gpio.h"
#include "print.h"


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

/* I2C peripheral devices have no minimum working clock frequency
   , however 100KHz is usually the baseline.
	 Please refer to the specific processor documentation to make sure 
	  the desired mode is supported.
*/
   


/*---------------------------------------------------------------------------------------
  function : cI2c()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cI2c::cI2c()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cI2c()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cI2c::~cI2c()
{
}

/*---------------------------------------------------------------------------------------
  function : scanI2cMaster()
    @brief    : We can use the following codes to find our devices’ I2C address.
                7bit addressing
    @param
    @return
----------------------------------------------------------------------------------------*/
void cI2c::scanI2cMaster()
{
#if 0
   uint8_t uError, uAddr;
   int nDevices;

   printk(LOG_TRACE, F("Scan for I2C devices\n"));

   nDevices = 0;
   for (uAddr = 1; uAddr < 127; uAddr++)
   {
      Wire.beginTransmission(uAddr);
      uError = Wire.endTransmission();
      if (uError == 0)
      {
         printk(LOG_INFO, F("2C device found at address 0x%02x\n"), uAddr);
         nDevices++;
      }
      else if (uError == 4)
      {
         printk(LOG_ERROR, F("Unknow error at address 0x%02x\n"), uAddr);
      }
   }
   if (nDevices == 0)
   {
      printk(LOG_DEBUG, F("<ESP32> No I2C devices found\n"));
   }
   else
   {
      printk(LOG_DEBUG, F("done\n"));
   }
#endif	 
}

/*---------------------------------------------------------------------------------------
  function : bProveI2C_Master()
    @brief    : prove
    @param
    @return
----------------------------------------------------------------------------------------*/
bool cI2c::bProveI2C_Master(uint8_t uAddr)
{
#if 0
   uint8_t uError;
   Serial.println("Prove for I2C devices");

   Wire.beginTransmission(uAddr);
   uError = Wire.endTransmission();
   if (uError == 0)
   {
      printk(LOG_TRACE, F("I2C device found at address 0x%02x\n"), uAddr);
      return true;
   }
#endif
   return false;
}

/*---------------------------------------------------------------------------------------
  function : setupI2cMaster()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cI2c::setupI2cMaster()
{
   printk(LOG_DEBUG, F("setupI2cMaster()\n"));
   //Wire.begin(I2C_SDA, I2C_SCL);
   Wire.begin();
   Wire.setClock(STD_CLK_I2C); 
}

/*---------------------------------------------------------------------------------------
  function : write_I2C()
    @brief    :  Master --> Slaver
    @param
    @return
----------------------------------------------------------------------------------------*/
void cI2c::write_I2C(uint8_t u8SlaveAddr, uint8_t u8Data)
{
  printk(LOG_DEBUG, F("write_I2C(Slave(0x%x), data(0x%x)\n"), u8SlaveAddr, u8Data);

	Wire.beginTransmission(u8SlaveAddr); /* to Slave Device  */
	Wire.write(u8Data); 								 /* transfer data */
	Wire.endTransmission();
}

/*---------------------------------------------------------------------------------------
  function : read_I2C()
    @brief    :  Master <-- Slaver
    @param
    @return
----------------------------------------------------------------------------------------*/
int cI2c::nRead_I2C(uint8_t u8SlaveAddr, uint8_t u8Size)
{
	int nData;
  printk(LOG_DEBUG, F("read_I2C(Slave(0x%x), size=%u)\n"), u8SlaveAddr, u8Size);

	Wire.requestFrom(u8SlaveAddr, u8Size);

	while (0 < Wire.available())
	{
	  nData = Wire.read();
	  printk(LOG_INFO, F("[I2C] read : %d\n"), nData);
	  return nData;
	}
	return -1;
}

/*---------------------------------------------------------------------------------------
  function : writeRead_I2C()
    @brief    :  Master <--> Slaver
    @param
    @return
----------------------------------------------------------------------------------------*/
void cI2c::writeRead_I2C(uint8_t u8SlaveAddr, uint8_t* pTxData, uint8_t u16TxSize, uint8_t* pRxData, uint8_t u16RxSize)
{
   printk(LOG_DEBUG, F("writeRead_I2C(Slave(0x%x), Tx(0x%x, %u), RxSize(%u) \n")
	 	, u8SlaveAddr, pTxData[0], u16TxSize, u16RxSize);

	// write
	Wire.beginTransmission(u8SlaveAddr);
	for(uint8_t i = 0; i < u16TxSize; i++)
	{
		Wire.write((byte)pTxData[i]); 
	}
	Wire.endTransmission();

  // read
	Serial.print("READ =");  
	Wire.requestFrom(u8SlaveAddr, u16RxSize);
	for(uint8_t i = 0; i < u16RxSize; i++)
	{
		if(Wire.available() > 0)
		{
			pRxData[i] = Wire.read();
			Serial.println(pRxData[i]);			
		}
	}
}


/*---------------------------------------------------------------------------------------
  function : RecoverI2cLockup()
    @brief    :
        * Issue a sequence of ten clock cycles on the I2C SCL i/o pin, without regard
        * to the normal I2C protocol.  This is intended to recover from a lock-up
        * condition whereby a slave is driving SDA low because it is stuck in mid-
        * transaction.  This condition should not occur during normal start-up from
        * power-up, but it may occur if the software is re-started (e.g. during a
        * debugging session).

        If a clock edge gets ��lost�� somehow, the slave device will never finish transferring the data
        and may end up continuously pulling the data line to an active low state.

        https://pebblebay.com/i2c-lock-up-prevention-and-recovery/

    @param
    @return
----------------------------------------------------------------------------------------*/
#define I2C_RECOVER_NUM_CLOCKS 10U    /* # clock cycles for recovery  */
#define I2C_RECOVER_CLOCK_FREQ STD_CLK_I2C /* clock frequency for recovery */
#define I2C_RECOVER_CLOCK_DELAY_US (1000000U / (2U * I2C_RECOVER_CLOCK_FREQ))

void cI2c::RecoverI2cLockup(void) 
{
   printk(LOG_TRACE, F("RecoverI2cLockup() start\n"));    

//   noInterrupts();
   //Disable I2C Peripheral 
   Wire.end();
//   interrupts();
  
   //Initialise pin direction and initial state (high). 
    pinMode(I2C_SCL, OUTPUT);
    digitalWrite(I2C_SCL, HIGH);    

   //Synthesise several clock cycles; leave i/o pin high.
   for (unsigned int i = 0U; i < I2C_RECOVER_NUM_CLOCKS; ++i)
   {
//      delay_us(I2C_RECOVER_CLOCK_DELAY_US);
      digitalWrite(I2C_SCL, LOW);   
//      delay_us(I2C_RECOVER_CLOCK_DELAY_US);
      digitalWrite(I2C_SCL, HIGH);    
   }
   printk(LOG_TRACE, F("RecoverI2cLockup() finish\n"));   

//   noInterrupts();
   // I2C restart  
   Wire.begin();     //7 비트 slave address. 생략할 경우 마스터로 인식 됨
   uint16_t I2C_CLOCK = (uint16_t)STD_CLK_I2C;
   Wire.setClock(I2C_CLOCK); // MAX 800kHz  
//   interrupts();
}


