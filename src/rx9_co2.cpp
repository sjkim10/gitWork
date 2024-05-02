/*****************************************************************************************
  i2c.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-02-24
  @ Author : Piquant
*****************************************************************************************/
#include <Arduino.h>
#include <Wire.h>
#include "RX9QR.h"

#include "i2c.h"
#include "gpio.h"
#include "co2_def.h"
#include "rx9_co2.h"
#include "print.h"


#ifdef USE_RX9_CO2

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

//CO2 calibrated number
/*you can take the data from RX-9 bottom side QR data #### of first 4 digits. 
  you type the data to cal_A as ###.#
*/
float g_fCalA = 372.1; 
/* following 4 digits after cal_A is cal_B, type the data to cal_B as ##.## */
float g_fCalB = 63.27;

// Thermister constant
float g_fResist0 = 15;

//Timing
unsigned int g_uTimeS = 0;
unsigned int g_uTimeS_Pre = 0;
unsigned int g_uTimeS_Set = 1;

volatile unsigned long g_ulTimer0; //ms

//CO2 Value
int g_nStatusSensor = 0;
unsigned int g_unCo2Ppm = 0;
unsigned int g_unCo2Step = 0;
float g_fEMF = 0;
float g_fTHER = 0;


RX9QR RX9(g_fCalA, g_fCalB, BASE_LINE, METI, MEIN, CR1, CR2, CR3, CR4);



/*---------------------------------------------------------------------------------------
  function : Rx9()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cRx9Co2::cRx9Co2()
{
}

/*---------------------------------------------------------------------------------------
  function : ~Rx9()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cRx9Co2::~cRx9Co2()
{
}

/*---------------------------------------------------------------------------------------
  function : init_Rx9_Co2()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cCo2::init_Rx9_Co2()
{
	//set the digital pin as output:
	pinMode(POWER_SWITCH_CO2, OUTPUT);
	digitalWrite(POWER_CO2_PIN, HIGH);  

#if 0
  pinMode(sw, INPUT_PULLUP); //Manual CAL용 스위치 핀모드 설정
  delay(1000);
  
  //parameter init
  //주요 파라미터들을 EEPROM의 값을 불러와서 계산에 사용하기 위해서 초기화 시켜줌
  
  cal_A = EEPROM.read(cal_A_ADD_01)*256 + EEPROM.read(cal_A_ADD_02) + (float)EEPROM.read(cal_A_ADD_03)/100; //ppm
  if(cal_A<0){cal_A = 0;} //ABC
  CANU = EEPROM.read(CANU_ADD_01)*256+(float)EEPROM.read(CANU_ADD_02); //ABC
  if(CANU<0){CANU = 0;} //ABC
  EMF_max = EEPROM.read(EMF_max_ADD_01)*256 + EEPROM.read(EMF_max_ADD_02) + (float)EEPROM.read(EMF_max_ADD_03)/100;  //ABC
  if(EMF_max<0){EMF_max = 0;} //ABC
  THER_max = EEPROM.read(THER_max_ADD_01)+EEPROM.read(THER_max_ADD_02)/100; //2CH
  if(THER_max<0){THER_max = 0;} //2CH
  THER_ini = EEPROM.read(THER_ini_ADD_01) + (float)EEPROM.read(THER_ini_ADD_02)/100;
  if(THER_ini<0){THER_ini = 0;} //2CH  
  //parameter init END
  //Reset on/off
  //3분 리셋을 할 것인지 말 것인지를 정하는 부분
  if(CANU<INIT_OFF || INIT_OFF == 0){
    Reset_mode = 1;
  }
  else{
    Reset_mode = 0;
  }
  
#endif
}

/*---------------------------------------------------------------------------------------
  function : turnOnOff_Rx9_Co2()
    @brief    : 
    @param
    @return
----------------------------------------------------------------------------------------*/
void cCo2::turnOnOff_Rx9_Co2(bool bOn)
{
   printk(LOG_DEBUG, F("turnOnOff_Rx9_Co2(%d)\n"), bOn);

	 digitalWrite(POWER_SWITCH_CO2, bOn);  
}

/*---------------------------------------------------------------------------------------
  function : calibrate_Rx9_Co2()
    @brief    : prove
    @param
    @return
----------------------------------------------------------------------------------------*/
bool cCo2::calibrate_Rx9_Co2(uint8_t uAddr)
{
   printk(LOG_DEBUG, F("calibrate_Rx9_Co2()\n"));

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
  function : u8GetStatus_Rx9_Co2()
    @brief    :
    	CO2(ppm) < 700 ppm --> Very good
    	CO2(ppm) < 1000 ppm --> good    	
    	CO2(ppm) < 2000 ppm --> bad
    	CO2(ppm) ≥ 2000 ppm --> Very bad
    @param
    @return
----------------------------------------------------------------------------------------*/
uint8_t cCo2::u8GetStatus_Rx9_Co2()
{
	uint8_t u8Status;

	unGetRx9_Co2();
	
	if(g_nStatusSensor == 0) u8Status = WARM_UP_M;
	else 
	{
		if(g_unCo2Ppm < 700) u8Status = VERY_GOOD_M;
		else if(g_unCo2Ppm < 1000) u8Status = GOOD_M;
		else if(g_unCo2Ppm < 2000) u8Status = BAD_M;
		else u8Status = VERY_BAD;
	}
	
	printk(LOG_DEBUG, F("u8GetStatus_Rx9_Co2(), u8Status = %u\n"), u8Status);
	
	return u8Status;
}


/*---------------------------------------------------------------------------------------
  function : unGetRx9_Co2()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
unsigned int cCo2::unGet_Rx9_Co2()
{
   printk(LOG_DEBUG, F("fGetRx9_Co2()\n"));
  // put your main code here, to run repeatedly:
  g_uTimeS = millis()/1000;
  if(g_uTimeS - g_uTimeS_Pre >= g_uTimeS_Set){
    g_uTimeS_Pre = g_uTimeS;
    //every 1 sec
    //read EMF data from RX-9, RX-9 Simple START-->
    g_fEMF = analogRead(EMF_PIN);
    delay(1);
    g_fEMF = g_fEMF / (RESOL_ADC - 1);
    g_fEMF = g_fEMF * VOLT_ADC;
    g_fEMF = g_fEMF / 6;
    g_fEMF = g_fEMF * 1000;
    // <-- read EMF data from RX-9, RX-9 Simple END 

    //read THER data from RX-9, RX-9 Simple START-->
    g_fTHER = analogRead(THER_PIN);
    delay(1);
    g_fTHER = 1/(C1+C2*log((g_fResist0*g_fTHER)/(RESOL_ADC-g_fTHER))+C3*pow(log((g_fResist0*g_fTHER)/(RESOL_ADC-g_fTHER)),3))-273.15;
    // <-- read THER data from RX-9, RX-9 Simple END
    
    g_nStatusSensor = RX9.status_co2();   //read status_sensor, status_sensor = 0 means warming up, = 1 means stable
    g_unCo2Ppm = RX9.cal_co2(g_fEMF,g_fTHER);    //calculation carbon dioxide gas concentration. 
    g_unCo2Step = RX9.step_co2();          //read steps of carbon dioixde gas concentration. you can edit the step range with cr1~cr4 above.

		return g_unCo2Ppm;
 	}
}	

#endif

