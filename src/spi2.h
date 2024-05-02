/*****************************************************************************************
spi2.h
~~~~~~~~~~~~~~~~~~~~~~~~~
	@ Project : AirQuant
	@ Date : 2024-02-23
	@ Author : Piquant
*****************************************************************************************/

#ifndef _SPI_TEST_H_
#define _SPI_TEST_H_

#include "define.h"
//#include <driver/gpio.h>
#include "TemplateSingleTon.h"


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
/* SPI
#define SPI_SS   PIN_SPI_SS    D10   
#define SPI_MOSI PIN_SPI_MOSI  D11
#define SPI_MISO PIN_SPI_MISO  D12
#define SPI_SCK  PIN_SPI_SCK   D13
*/


/*****************************************************************************************
 *                              cSpi  Class
 *****************************************************************************************/
class cSpi : public TemplateSingleton<cSpi>
{
public:
	cSpi();
	virtual ~cSpi();

	void initSpiMaster();
	void sendReadSpi(uint8_t* p8Tx, uint8_t u8Size, uint8_t* p8Rx);
};

#endif /* _SPI_TEST_H_ */

