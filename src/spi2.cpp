/*****************************************************************************************
spi2.cpp
 -------------------------------------
	@ Project : AirQuant
	@ Date : 2024-02-23
	@ Author : Piquant
*****************************************************************************************/
#include <Arduino.h>
#include <SPI.h>

#include "spi2.h"
#include "print.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
  function : cSpi()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cSpi::cSpi()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cSpi()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cSpi::~cSpi()
{
}

/*---------------------------------------------------------------------------------------
  function : initSpiMaster()
    @brief    : 
    @param
    @return
----------------------------------------------------------------------------------------*/
void cSpi::initSpiMaster()
{
	printk(LOG_DEBUG, F("initSpiMaster()\n"));

	SPI.begin();
	//SPI.setDataMode(SPI_MODE0); // Output Edge (Falling), Data Capture(Rising)
	//SPI.setClockDivider(SPI_CLOCK_DIV16); //speed : 2/4/8/16/32/64/128
	//digitalWrite(SS, LOW);
	
#if 0	// no need
	// set the digital pin as output:
	pinMode(MISO, OUTPUT);
	pinMode(MOSI, INPUT);	
	pinMode(SCK, INPUT);	
	pinMode(SS, INPUT);	
#endif
}

/*---------------------------------------------------------------------------------------
  function : sendReadSpi()
    @brief    : 
    @param
    @return
----------------------------------------------------------------------------------------*/
void cSpi::sendReadSpi(uint8_t* p8Tx, uint8_t u8Size, uint8_t* p8Rx)
{
	//digitalWrite(SS, HIGH);
	for(uint8_t i = 0; i < u8Size; i++)
	{
		p8Rx[i] = SPI.transfer(p8Tx[i]);
	}
}

