/*****************************************************************************************
task_ble_beacon.cpp
 -------------------------------------
	@ Project : AirQuant
	@ Date : 2024-02-19
	@ Author : Piquant
*****************************************************************************************/

#include <Arduino.h>
#include <ArduinoBLE.h>

#include "timer.h"
#include "task.h"
#include "system.h"
#include "utils.h"
#include "ble_beacon.h"
#include "init.h"
#include "config.h"
#include "led2.h"
#include "scd30_co2.h"

#include "print.h"



/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#define BLE_DEV_NAME "PQBLE"


#ifdef NO_CONNECT_ADVERTISING
#if 0
stUserData_st g_stUserData_st = 
{
	{2, 1, 6}, 
	{14, 3, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
	{4, 3, 0x43, 0x4f, 0x32}
};
#endif
stManufacturingData_st g_stManufacturingData_st = {0};

#else
BLEService getCO2Service("180C");  // User defined service
BLEStringCharacteristic getCO2ServiceCharacteristic(
	"2A56",  // standard 16-bit characteristic UUID
  BLERead | BLENotify | BLEWrite, // BLE properties
  MAX_SENSOR_DATA // remote clients will only be able to read this
);
volatile char g_sSensorData[MAX_SENSOR_DATA] = {0};
volatile bool g_bConnectedBle = false;


/*---------------------------------------------------------------------------------------
  function : blePeripheralConnectHandler()
    @brief :
    @param
    @return
----------------------------------------------------------------------------------------*/
void blePeripheralConnectHandler(BLEDevice central) 
{
  // central connected event handler
	getCO2ServiceCharacteristic.setValue((char*)g_sSensorData); // Set sensor data

	cLed::set3Led(255, 255, 255);

	printk(LOG_INFO, F("[BLE_Dev%u]connected event, Central :%s\n"), DEVICE_ID, central.address().c_str());	
}

/*---------------------------------------------------------------------------------------
  function : blePeripheralDisconnectHandler()
  	@brief :
    @param
    @return
----------------------------------------------------------------------------------------*/
void blePeripheralDisconnectHandler(BLEDevice central) 
{
	cLed::set3Led(255, 255, 255);

	printk(LOG_INFO, F("[BLE_Dev%u]Disconnected event, Central :%s\n"), DEVICE_ID, central.address().c_str());	
}

/*---------------------------------------------------------------------------------------
  function : getCO2ServiceCharacteristicWritten()
  	@brief :
    @param
    @return
----------------------------------------------------------------------------------------*/
void getCO2ServiceCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) 
{
	int nRdVal;

  // central wrote new value to characteristic, update LED
	printk(LOG_INFO, F("[BLE_Dev%u]Characteristic event, written : \n"));	

  if (getCO2ServiceCharacteristic.value()) 
	{
		nRdVal = atoi(getCO2ServiceCharacteristic.value().c_str());
		printk(LOG_ALWAYS, F("<Read> Val = %s(%d)\n"), getCO2ServiceCharacteristic.value().c_str(), nRdVal);
		//printf("<Read> Val = %s(%d)\n", getCO2ServiceCharacteristic.value().c_str(), nRdVal);
	  printk(LOG_INFO, F("[BLE_Dev%u]LED On\n"));			
	  ((cLed*)g_stInst.pLed)->setAdvLed(HIGH);
  } 
	else {
	  printk(LOG_INFO, F("[BLE_Dev%u]LED Off\n"));			
	  ((cLed*)g_stInst.pLed)->setAdvLed(LOW);
  }
}
#endif


/* ====================== Class ================================ */

/*---------------------------------------------------------------------------------------
  function : cBleBeacon()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cBleBeacon::cBleBeacon()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cBleBeacon()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cBleBeacon::~cBleBeacon()
{
}


#ifdef NO_CONNECT_ADVERTISING
/*---------------------------------------------------------------------------------------
function : bInitAdj_NoConnected()
  @brief    : broadcasting
  @param
  @return
----------------------------------------------------------------------------------------*/
bool cBleBeacon::bInitAdj_NoConnected() 
{
	uint8_t* p = (uint8_t*)&g_stManufacturingData_st;
	uint16_t u16Size = sizeof(g_stManufacturingData_st);
	g_stManufacturingData_st.u16DevId = DEVICE_ID;

	showAdvMsg(SHOW_HEX_ALL, p, u16Size);

  // begin initialization
  if (!BLE.begin()) {
		printk(LOG_ERROR, F("[BLE_Dev%u] <Error>BLE.begin()\n"), DEVICE_ID);
	  return false;
  }
  // set advertised local name and device name
  BLE.setLocalName(BLE_DEV_NAME);
  BLE.setDeviceName(BLE_DEV_NAME);

	g_stManufacturingData_st.u16DevId = DEVICE_ID;

  if(!BLE.setManufacturerData(p, u16Size)) {
		printk(LOG_ERROR, F("[BLE_Dev%u] <Error>BLE.setManufacturerData()\n"), DEVICE_ID);
		return false;
  }

  //BLE.setAdvertisingInterval(320*3); // 200 * 0.625 ms  
  // start advertising
  if(!BLE.advertise()){
		printk(LOG_ERROR, F("[BLE_Dev%u] <Error>BLE.advertise()\n"), DEVICE_ID);
		return false;
 	}
	return true;
}


#else
/*---------------------------------------------------------------------------------------
function : bInitAdj_Connected()
  @brief    : broadcasting
  @param
  @return
----------------------------------------------------------------------------------------*/
bool cBleBeacon::bInitAdj_Connected() 
{
	if (!BLE.begin()) { 	// initialize BLE
		printk(LOG_ERROR, F("[BLE_Dev%u] <Error>BLE.begin()\n"), DEVICE_ID);
		return false;
	}

	BLE.setLocalName(BLE_DEV_NAME);  // Set name for connection
	BLE.setAdvertisedService(getCO2Service); // Advertise service
	getCO2Service.addCharacteristic(getCO2ServiceCharacteristic); // Add characteristic to service
	BLE.addService(getCO2Service); // Add service

	// assign event handlers for connected, disconnected to peripheral
	BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
	BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

	// assign event handlers for characteristic
	getCO2ServiceCharacteristic.setEventHandler(BLEWritten, getCO2ServiceCharacteristicWritten);
	// set an initial value for the characteristic
	getCO2ServiceCharacteristic.setValue((char*)g_sSensorData); // Set sensor data	

	BLE.advertise();  // Start advertising

	printk(LOG_INFO, F("[BLE_Dev%u]Waiting for connectins ...\n"), DEVICE_ID);

	return true;
}
#endif


/*---------------------------------------------------------------------------------------
function : bInitBle()
  @brief    : 
  @param
  @return
----------------------------------------------------------------------------------------*/
bool cBleBeacon::bInitBle() 
{
	bool bOk = true;
	
#ifdef NO_CONNECT_ADVERTISING
	printk(LOG_DEBUG, F("[BLE_Dev%u] bInitAdj_NoConnected()\n"), DEVICE_ID);
	bOk = bInitAdj_NoConnected();
#else
	printk(LOG_DEBUG, F("[BLE_Dev%u] bInitAdj_Connected()\n"), DEVICE_ID);
	bOk = bInitAdj_Connected();
#endif

	return bOk;
}


/*---------------------------------------------------------------------------------------
function : exitBle()
  @brief    : 
  @param
  @return
----------------------------------------------------------------------------------------*/
void cBleBeacon::exitBle()
{
}


#ifdef NO_CONNECT_ADVERTISING
/*---------------------------------------------------------------------------------------
function : bSendAdvertising_NoConnected()
  @brief    : 
  @param
  @return
----------------------------------------------------------------------------------------*/
bool cBleBeacon::bSendAdvertising_NoConnected() 
{
	uint8_t* p = (uint8_t*)&g_stManufacturingData_st;
	uint16_t u16Size = sizeof(g_stManufacturingData_st);
	g_stManufacturingData_st.u16DevId = DEVICE_ID;

  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }

	BLE.stopAdvertise();
	//BLE.setManufacturerData(g_mData, sizeof(g_mData));
	BLE.setManufacturerData(p, u16Size);

	// start advertising
	BLE.advertise();
	showAdvMsg(SHOW_HEX_ALL, p, u16Size);

	return true;
}

#else
/*---------------------------------------------------------------------------------------
function : bSendAdvertising_Connected()
  @brief    : 
  @param
  @return
----------------------------------------------------------------------------------------*/
bool cBleBeacon::bSendAdvertising_Connected()
{
	// listen for BLE peripherals to connect:
	BLEDevice central = BLE.central();
	uint8_t u8P;
	static char _sPreData[MAX_SENSOR_DATA]={0};
	static bool _bConnect = true;
	static bool _bDisConnect = true;
	unsigned long dwTime = 20;
	// if a central is connected to peripheral:
	if (central) {
		Serial.print("Connected to central MAC : ");
		// print the central's MAC address:
		Serial.println(central.address());

		while(central.connected())
    {
			static unsigned long _ulStartMs = millis();    
			//turn on the LED to indicate the connection:
			if(_bConnect) {
				_bConnect = false;
				g_bConnectedBle = true;
				((cLed*)g_stInst.pLed)->setAdvLed(HIGH);
			}
			_bDisConnect = true;			

			if((millis() - _ulStartMs) > 4000)
			{
				_ulStartMs = millis();			
				((cTimer*)g_stInst.pTimer)->bResetWDT();			
			}

#if 0
			static unsigned int unVal = 12;
			sprintf((char*)g_sSensorData, "%u", unVal++);
#endif
			if(memcmp((char*)g_sSensorData, _sPreData, MAX_SENSOR_DATA) != 0)
			{
				getCO2ServiceCharacteristic.setValue((char*)g_sSensorData);
				cLed::set3ResultLed(g_stSensor.u8Status, g_stSensor.fBattery);
				memcpy(_sPreData, (char*)g_sSensorData, MAX_SENSOR_DATA);
				printk(LOG_ALWAYS, F("<to Mobile> g_sSensorData = %s(%u), %u, %u, %ld\n"), g_sSensorData, u8P, _bConnect, _bDisConnect, dwTime);
			}
		}
  }
	else 
	{
		if((_bConnect != true) && (_bDisConnect == true))
		{
   		_bConnect = true;
			_bDisConnect = false;
			g_bConnectedBle = false;

			//dwTime = 5000;
			// when the central disconnects, turn off the LED:
			//digitalWrite(LED_BUILTIN, LOW);
			//((cLed*)g_stInst.pLed)->setPwrLed(OFF);
	    ((cLed*)g_stInst.pLed)->setAdvLed(LOW);
			Serial.print(F("Disconnected from central: "));
			Serial.println(central.address());
			//printk(LOG_DEBUG, F("%u, %u, %ld\n"), _bConnect, _bDisConnect, dwTime);
		}
	}
//	showAdvMsg(SHOW_HEX_ALL, p, u16Size);

	return true;
}
#endif

/*---------------------------------------------------------------------------------------
function : bSendAdvertising()
  @brief    : 
  @param
  @return
----------------------------------------------------------------------------------------*/
bool cBleBeacon::bSendAdvertising()
{
	static bool _bOne = true;
	bool bOk = true;
	
#ifdef NO_CONNECT_ADVERTISING
	if(_bOne)
	{
		_bOne = false;
		printk(LOG_DEBUG, F("[BLE_Dev%u] bSendAdvertising_NoConnected()\n"), DEVICE_ID);
	}
	bOk = bSendAdvertising_NoConnected();
#else
	if(_bOne)
	{
		_bOne = false;
		printk(LOG_DEBUG, F("[BLE_Dev%u] bSendAdvertising_Connected()\n"), DEVICE_ID);
	}
	bOk = bSendAdvertising_Connected();
#endif

	return bOk;
}


/*---------------------------------------------------------------------------------------
function : bGetBleState()
  @brief    : 
  @param
  @return
----------------------------------------------------------------------------------------*/
bool cBleBeacon::bGetBleState()
{

	return true;
}

/*---------------------------------------------------------------------------------------
function : bGetBleMac()
  @brief    : 
  @param
  @return
----------------------------------------------------------------------------------------*/
bool cBleBeacon::bGetBleMac(bool bShow)
{
	return true;
}

/*---------------------------------------------------------------------------------------
function : bGetRspData()
  @brief    : 
  @param
  @return
----------------------------------------------------------------------------------------*/
bool cBleBeacon::bGetRspData()
{
	return true;
}

bool cBleBeacon::bSetAdvData()
{

	return true;
}


