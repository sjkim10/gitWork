/*****************************************************************************************
init.cpp
 -------------------------------------
	@ Project : AirQuant
	@ Date : 2024-02-19
	@ Author : Piquant
*****************************************************************************************/
#include <Arduino.h>

#include "system.h"
#include "timer.h"
#include "time_func.h"
#include "ble_beacon.h"
#include "utils.h"

#include "task.h"
#include "init.h"
#include "opmode.h"
#include "led2.h"
#include "spi2.h"

#include "config.h"
#include "gpio.h"
#include "i2c.h"
#include "packet.h"
#include "opmode.h"
#include "scd30_co2.h"
#include "uart.h"
#include "test.h"

#include "print.h"



/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
stInsatnce_t g_stInst;

#define WDT_TIME   5 /* 5 second */



/*---------------------------------------------------------------------------------------
  function : cFan()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cInit::cInit()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cFan()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cInit::~cInit()
{
}

/*---------------------------------------------------------------------------------------
  function : bInit()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
bool cInit::bInit()
{
	cInit* pInit;
	if ((pInit = cInit::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> cInit::getInstance() = NULL \n"));
		return false;
	}
	pInit->getinstance();
	pInit->initDevice();
	pInit->initTasks();

//	cTimer::bAddCurrentTaskWDT((char*)"main");

	return true;
}

/*---------------------------------------------------------------------------------------
  function : getinstance()
    @brief :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cInit::getinstance()
{
	if ((g_stInst.pBleBeacon = (void *)cBleBeacon::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pBleBeacon \n"));
		return;
	}
	if ((g_stInst.pTimer = (void *)cTimer::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pTimer \n"));
		return;
	}
	if ((g_stInst.pTmFunc = (void *)cTimeFunc::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pTmFunc \n"));
		return;
	}
	if ((g_stInst.pTask = (void *)cTask::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pTask \n"));
		return;
	}
	if ((g_stInst.pUtils = (void *)cUtils::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pUtils \n"));
		return;
	}

	if ((g_stInst.pSystem = (void *)cSystem::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pSystem \n"));
		return;
	}

	if ((g_stInst.pLed = (void *)cLed::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pLed \n"));
		return;
	}

	if ((g_stInst.pCo2 = (void *)cScd30Co2::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pCo2 \n"));
		return;
	}

	if ((g_stInst.pI2c = (void *)cI2c::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pI2c \n"));
		return;
	}
	if ((g_stInst.pGpio = (void *)cGpio::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pGpio \n"));
		return;
	}

	if ((g_stInst.pPacket = (void *)cPacket::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pI2c \n"));
		return;
	}
	if ((g_stInst.pOpMode = (void *)cOpMode::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pGpio \n"));
		return;
	}
	if ((g_stInst.pUart = (void *)cUart::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pUart \n"));
		return;
	}
	if ((g_stInst.pTest = (void *)cTest::GetInstance()) == NULL)
	{
		printk(LOG_ERROR, F("<Error> getInstance() : pTest \n"));
		return;
	}
	
}

/*---------------------------------------------------------------------------------------
  function : initDevice()
    @brief :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cInit::initDevice()
{
	//   printk(LOG_TRACE, F("[1] Free internal heap : %u, %u\n")
	//   	  , ESP.getFreeHeap(), ESP.getFreePsram());
	((cGpio*)g_stInst.pGpio)->initGpio();
	((cLed*)g_stInst.pLed)->initLed();
	((cSpi*)g_stInst.pSpi)->initSpiMaster();
	((cI2c*)g_stInst.pI2c)->setupI2cMaster();
	((cI2c*)g_stInst.pI2c)->scanI2cMaster();
	((cTimer*)g_stInst.pTimer)->bInitWDT(WDT_TIME);
	((cSystem*)g_stInst.pSystem)->initSystem();	 
	((cScd30Co2*)g_stInst.pCo2)->initScd30_Co2();	 	

	//   printk(LOG_TRACE, F("[initDevice] Free internal heap : %u, %u\n")
	//   	, ESP.getFreeHeap(), ESP.getFreePsram());
}


/*---------------------------------------------------------------------------------------
  function : showVersion()
    @brief :
    @param
    @return
----------------------------------------------------------------------------------------*/
char *cInit::showVersion()
{
	static char sVer[100];

	sprintf(g_stVersion.sBuildDate, "%s", BUILD_DATE);
	g_stVersion.u8Major = VER_MAJOR;
	g_stVersion.u8Minor = VER_MINOR;	
	g_stVersion.u8Minor2 = VER_MINOR2;		

   printf("-------------------------------------------------------------------\n");
   sprintf(sVer, "Version [%s] %d.%d.%d", BUILD_DATE, VER_MAJOR, VER_MINOR, VER_MINOR2);
   printf("%s\n", sVer);
   printf("-------------------------------------------------------------------\n");

   return sVer;
}


/*---------------------------------------------------------------------------------------
  function : initTasks()
    @brief
    @param
    @return
----------------------------------------------------------------------------------------*/
void cInit::initTasks()
{
   printk(LOG_TRACE, F("initTasks\n"));

   ((cTask *)g_stInst.pTask)->CreateTasks();
}


