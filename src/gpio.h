/*****************************************************************************************
gpio.h
~~~~~~~~~~~~~~~~~~~~~~~~~
	@ Project : AirQuant
	@ Date : 2024-02-19
	@ Author : Piquant
*****************************************************************************************/

#ifndef _GPIO_TEST_H_
#define _GPIO_TEST_H_

#include <pins_arduino.h>
#include "define.h"
#include "TemplateSingleTon.h"


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

//Serial interfaces
// Serial (EDBG)
#define UART_RX2   PIN_SERIAL_RX
#define UART_TX2   PIN_SERIAL_TX

// I2C
#define I2C_SDA PIN_WIRE_SDA
#define I2C_SCL PIN_WIRE_SCL
#define I2C_SDA2 PIN_WIRE_SDA1
#define I2C_SCL2 PIN_WIRE_SCL1

// SPI
#define SPI_SS   PIN_SPI_SS
#define SPI_MOSI PIN_SPI_MOSI
#define SPI_MISO PIN_SPI_MISO
#define SPI_SCK  PIN_SPI_SCK

// Digital pins
#define DIO_0  D0
#define DIO_1  D1
#define DIO_2  D2 //PWM
#define DIO_3  D3 //PWM 
#define DIO_4  D4
#define DIO_5  D5 //PWM
#define DIO_6  D6 //PWM
#define DIO_7  D7
#define DIO_8  D8
#define DIO_9  D9  //PWM
#define DIO_10 D10 //PWM
#define DIO_11 D11 //PWM
#define DIO_12 D12 //PWM
#define DIO_13 D13

// Analog pins
#define AIO_0  PIN_A0
#define AIO_1  PIN_A1
#define AIO_2  PIN_A2
#define AIO_3  PIN_A3
#define AIO_4  PIN_A4
#define AIO_5  PIN_A5
#define AIO_6  PIN_A6
#define AIO_7  PIN_A7

// LEDs
//#define PIN_LED     (13u)
//#define LED_BUILTIN PIN_LED Active HIGH
//#define LEDR        (22u) Active Low
//#define LEDG        (23u) Active Low
//#define LEDB        (24u) Active Low
//#define LED_PWR     (25u) Active HIGH


// used pin
#define RED_PIN   		D8  //Bad
#define GREEN_PIN 		D9  //Good
#define BLUE_PIN  		D10 //Excellent

/* LED(long -> + : Red) */
#define LED_PWR_PIN				D11 //Acitve Low
#define LED_ADV_PIN				D7  //Tx Advertising Data (No PWM)
#define POWER_SWITCH_CO2  D5
#define READY_CO2         D12

//#define I2C_SDA  PIN_WIRE_SDA
//#define I2C_SCL  PIN_WIRE_SCL
//LED_BUILTIN

#ifdef USE_RX9_CO2
#define EMF_PIN  AIO_0  //0 RX-9 E with A0 of arduino
#define THER_PIN AIO_1  //1 RX-9 T with A1 of arduino
#endif

#define BATTERY_LEVEL_PIN  AIO_2 //Battery Level


#define ON   true
#define OFF  false

/*****************************************************************************************
 *                              cGpio  Class
 *****************************************************************************************/
class cGpio : public TemplateSingleton<cGpio>
{
public:
	cGpio();
	virtual ~cGpio();

	void initGpio();
	void setPowerCo2(bool bOn);
	void readAnalog(float* fA1, float* fA2);
};

#endif /* _GPIO_TEST_H_ */

