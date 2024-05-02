/*****************************************************************************************
  system.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-02-24
  @ Author : Piquant
*****************************************************************************************/
//#include <Arduino_FreeRTOS.h>
//#include <RTCZero.h>
#include <mbed.h>
#include "mbed/drivers/include/drivers/AnalogIn.h"
#include "nrf_saadc.h"

#include "gpio.h"
#include "timer.h"
#include "system.h"
#include "utils.h"
#include "print.h"
#include "task.h"
#include "init.h"
#include "led2.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#define DAY_RTC    26
#define MONTH_RTC  2
#define YEAR_RTC   24
#define HOUR_RTC   1
#define MIN_RTC    23
#define SEC_RTC    0


/* Create an rtc object */
//RTCZero rtc;

/*---------------------------------------------------------------------------------------
  function : cWiFiDev()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cSystem::cSystem()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cWiFiDev()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cSystem::~cSystem()
{
}

/*---------------------------------------------------------------------------------------
  function : initSystem()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::initSystem()
{
	configADC();
	
	printk(LOG_ALWAYS, F("initSystem \n"));	

//	initRTC();
}

/*---------------------------------------------------------------------------------------
  function : configADC()
    @brief    :
    <	nrf_saadc_gain_t>
			NRF_SAADC_GAIN1_6 = SAADC_CH_CONFIG_GAIN_Gain1_6,
			NRF_SAADC_GAIN1_5 = SAADC_CH_CONFIG_GAIN_Gain1_5,
			NRF_SAADC_GAIN1_4 = SAADC_CH_CONFIG_GAIN_Gain1_4,
			NRF_SAADC_GAIN1_3 = SAADC_CH_CONFIG_GAIN_Gain1_3,
			NRF_SAADC_GAIN1_2 = SAADC_CH_CONFIG_GAIN_Gain1_2,
			NRF_SAADC_GAIN1 = SAADC_CH_CONFIG_GAIN_Gain1,
			NRF_SAADC_GAIN2 = SAADC_CH_CONFIG_GAIN_Gain2,
			NRF_SAADC_GAIN4 = SAADC_CH_CONFIG_GAIN_Gain4

   <nrf_saadc_reference_t>
			NRF_SAADC_REFERENCE_INTERNAL;
			NRF_SAADC_REFERENCE_VDD4;			

			Internal reference (0.6 V)
			Gain : 1/5

    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::configADC()
{
  // change the resolution to 12 bits and read BATTERY_LEVEL_PIN(AIO_2)
  analogReadResolution(12);
	printk(LOG_ALWAYS, F("initSystem : analogReadResolution(12)\n"));	
	
	nrf_saadc_reference_t reference = NRF_SAADC_REFERENCE_INTERNAL;
	nrf_saadc_gain_t gain = NRF_SAADC_GAIN1_5; /* 1/6 */
	adcCurrentConfig.reference = reference;
	adcCurrentConfig.gain = gain;
	analogUpdate();

//	initRTC();
}

/*---------------------------------------------------------------------------------------
  function : u16GetBatteryLevel()
    @brief    :  nano 33 bale : 3.3V. 12bit ADC

      VCC --- 100K --- [ADC] --- 300K --- GND
 
	    Internal reference (0.6 V)
    	Gain : 1/5

   (1) R : 300K / (100K + 300K)
     
		   ≥ 4095 : 4.0V
		   ≤ 3380 : 3.3V

 	(2) R : 200K / (100K + 200K)
		
			≥ 4095 : 4.5V
			≤ 3000 : 3.3V
		

    @param
    @return
----------------------------------------------------------------------------------------*/
uint16_t cSystem::u16GetBatteryLevel()
{
	int nVal = analogRead(BATTERY_LEVEL_PIN);
	//int volt = map(val,0,1023,0,5000);
	//int nVolt = map(n8Val,0,2047,0,3300);
	int nVolt = nVal * (1/4096) * 0.6 * 5 * (4/3);
	printk(LOG_ALWAYS, F("Battery Level : %d(%d mV)\n"), nVal, nVolt);
}


/*---------------------------------------------------------------------------------------
  function : initRTC()
    @brief    :
			#define DAY_RTC    26
			#define MONTH_RTC  2
			#define YEAR_RTC   24
			#define HOUR_RTC   1
			#define MIN_RTC    23
			#define SEC_RTC    0    
    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::initRTC()
{
#if 0   
	rtc.begin(); // initialize RTC
	// set the current time 8 August 2024 1:23 AM
	rtc.setDate(DAY_RTC, MONTH_RTC, YEAR_RTC);
	rtc.setTime(HOUR_RTC, MIN_RTC, SEC_RTC);
	printk(LOG_ALWAYS, F("set the current time 26 Feb 2024 1:23 AM\n"));	
#endif   
}

/*---------------------------------------------------------------------------------------
  function : gotoSleep()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::gotoSleep()
{
	printk(LOG_ALWAYS, F("goto Sleep\n"));

//	rtc.standbyMode();
}

/*---------------------------------------------------------------------------------------
  function : setWakeupRTC()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::setWakeupRTC()
{
	printk(LOG_ALWAYS, F("setWakeupRTC(%u sec)\n"), SEC_RTC+10);	
#if 0
	rtc.setAlarmTime(HOUR_RTC, MIN_RTC, SEC_RTC+10);
	rtc.enableAlarm(rtc.MATCH_HHMMSS);
	rtc.attachInterrupt(doWakeupRTC);
	//rtc.standbyMode();
#endif   
}

/*---------------------------------------------------------------------------------------
  function : doWakeupRTC()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::doWakeupRTC()
{
	((cLed*)g_stInst.pLed)->setOnboardLed(HIGH);

	printk(LOG_ALWAYS, F("doWakeupRTC()\n"));	
}

/*---------------------------------------------------------------------------------------
  function : print_reset_reason()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::print_reset_reason(uint8_t /*RESET_REASON*/ reason)
{
   switch (reason)
   {
   case 1:
      printk(LOG_ALWAYS, F("POWERON_RESET"));
      break; /**<1,  Vbat power on reset*/
   case 3:
      printk(LOG_ALWAYS, F("SW_RESET"));
      break; /**<3,  Software reset digital core*/
   case 4:
      printk(LOG_ALWAYS, F("OWDT_RESET"));
      break; /**<4,  Legacy watch dog reset digital core*/
   case 5:
      printk(LOG_ALWAYS, F("DEEPSLEEP_RESET"));
      break; /**<5,  Deep Sleep reset digital core*/
   case 6:
      printk(LOG_ALWAYS, F("SDIO_RESET"));
      break; /**<6,  Reset by SLC module, reset digital core*/
   case 7:
      printk(LOG_ALWAYS, F("TG0WDT_SYS_RESET"));
      break; /**<7,  Timer Group0 Watch dog reset digital core*/
   case 8:
      printk(LOG_ALWAYS, F("TG1WDT_SYS_RESET"));
      break; /**<8,  Timer Group1 Watch dog reset digital core*/
   case 9:
      printk(LOG_ALWAYS, F("RTCWDT_SYS_RESET"));
      break; /**<9,  RTC Watch dog Reset digital core*/
   case 10:
      printk(LOG_ALWAYS, F("INTRUSION_RESET"));
      break; /**<10, Instrusion tested to reset CPU*/
   case 11:
      printk(LOG_ALWAYS, F("TGWDT_CPU_RESET"));
      break; /**<11, Time Group reset CPU*/
   case 12:
      printk(LOG_ALWAYS, F("SW_CPU_RESET"));
      break; /**<12, Software reset CPU*/
   case 13:
      printk(LOG_ALWAYS, F("RTCWDT_CPU_RESET"));
      break; /**<13, RTC Watch dog Reset CPU*/
   case 14:
      printk(LOG_ALWAYS, F("EXT_CPU_RESET"));
      break; /**<14, for APP CPU, reseted by PRO CPU*/
   case 15:
      printk(LOG_ALWAYS, F("RTCWDT_BROWN_OUT_RESET"));
      break; /**<15, Reset when the vdd voltage is not stable*/
   case 16:
      printk(LOG_ALWAYS, F("RTCWDT_RTC_RESET"));
      break; /**<16, RTC Watch dog reset digital core and rtc module*/
   default:
      printk(LOG_ALWAYS, F("NO_MEAN"));
   }
}


/*---------------------------------------------------------------------------------------
  function : print_wakeup_reason()
    @brief    : Method to print the reason by which ESP32 has been awaken from sleep
    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::print_wakeup_reason()
{

}

/*---------------------------------------------------------------------------------------
  function : gotoDeepSleep()
    @brief    :  Device will sleep for nSec seconds.
    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::PeriodDeepSleep(uint8_t nSec)
{
}

/*---------------------------------------------------------------------------------------
  function : gotoDeepSleep()
    @brief    :  it will sleep forever unless hardware reset occurs.
    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::gotoDeepSleep()
{
}

#if 0
/*---------------------------------------------------------------------------------------
  function : setWakeupTimer()
    @brief    : (1) we configure the wake up source
    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::setWakeupTimer(uint8_t nSec)
{
}

/*---------------------------------------------------------------------------------------
  function : setWakeupExt()
    @brief    : 
         esp32-c3 does not support rtc wakeup. 
         esp_deep_sleep_enable_gpio_wakeup should work:
            esp_deep_sleep_enable_gpio_wakeup(1 << 9,ESP_GPIO_WAKEUP_GPIO_HIGH);

    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::setWakeupExt(uint8_t uExt)
{
#if 0
   if (uExt == EXT0_TYPE)
   {
      /* For ext0 */
      esp_sleep_enable_ext0_wakeup(WAKEUP_EXT0, 1); // 1 = High, 0 = Low
   }
   else
   {
#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex
      /* For ext1 */
      esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
   }
#endif	 
}

#if 0 // usage
	esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,1); //1 = High, 0 = Low

	//If you were to use ext1, you would use it like
	//esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);

	//Go to sleep now
	Serial.println("Going to sleep now");
	esp_deep_sleep_start();
	Serial.pri
	ntln("This will never be printed");

#endif
#else

/*---------------------------------------------------------------------------------------
  function : setWakeupGpio()
    @brief    : 
         esp32-c3 does not support rtc wakeup. 
         esp_deep_sleep_enable_gpio_wakeup should work:
            esp_deep_sleep_enable_gpio_wakeup(1 << 9,ESP_GPIO_WAKEUP_GPIO_HIGH);

    @param
    @return
----------------------------------------------------------------------------------------*/
void cSystem::setWakeupGpio(uint8_t u8GpioPin/*9*/)
{
#if 0
    esp_deep_sleep_enable_gpio_wakeup(1 << u8GpioPin,ESP_GPIO_WAKEUP_GPIO_HIGH);
#endif

}

#endif

