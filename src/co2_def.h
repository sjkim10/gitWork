/*****************************************************************************************
co2_def.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _CO2_DEF_TEST_H_
#define _CO2_DEF_TEST_H_

#include "define.h"
#include "TemplateSingleTon.h"


#ifdef USE_RX9_CO2

//https://colorscripter.com/s/Jnrf03K
//https://gamjathebest.tistory.com/8

/*
*  RX-9 have 4 pin
*  E: EMF
*  T: Thermistor for sensor
*  G: GND
*  V: 3.3V > 200 mA
*/

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#if 0
#define EMF_PIN  AIO_0  //0 RX-9 E with A0 of arduino
#define THER_PIN AIO_1  //1 RX-9 T with A1 of arduino
#endif
#define VOLT_ADC 3.3 //5
#define RESOL_ADC 1024
#define BASE_LINE 432
#define METI	60
#define MEIN	120 //Automotive: 120, Home or indoor: 1440

//CO2 calibrated number
extern float g_fCalA;
extern float g_fCalB;

//CO2 Step range
#define CR1  700      // BASE_LINE ~ CR1
#define CR2  1000     // CR1 ~ CR2
#define CR3  2000     // CR2 ~ CR3
#define CR4  4000     // CR3 ~ CR4 and over CR4

// Thermister constant
// RX-9 have thermistor inside of sensor package. this thermistor check the temperature of sensor to compensate the data
// don't edit the number
#define C1 0.00230088
#define C2 0.000224
#define C3 0.00000002113323296
extern float g_fResist0;

//Timing
extern unsigned int g_uTimeS;
extern unsigned int g_uTimeS_Pre;
extern unsigned int g_uTimeS_Set;

extern volatile unsigned long g_ulTimer0; //ms

//CO2 Value
extern int g_nStatusSensor;
extern unsigned int g_unCo2Ppm;
extern unsigned int g_unCo2Step;
extern float g_fEMF;
extern float g_fTHER;

#endif

/*=================================*/
#ifdef USE_SCD30_CO2

#endif


/*=================================*/

enum
{
	WARM_UP_M = 0,/*Warm up*/
	VERY_GOOD_M,  /*Very Good*/
	GOOD_M,       /*Good*/
	BAD_M,        /*Bad*/
	VERY_BAD      /*Very Bad*/
};
	

#endif /* _CO2_DEF_TEST_H_ */

