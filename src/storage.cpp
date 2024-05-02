/*****************************************************************************************
  cStorage.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-03-05
  @ Author : Piquant
*****************************************************************************************/
#include <Arduino.h>
#include "KVStore.h"
#include "kvstore_global_api.h"

#include "storage.h"
#include "print.h"


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

#ifdef USE_NOINIT
//__NOINIT_ATTR 
volatile stSwReset_t g_stSwReset = {0};
#else
stSwReset_t g_stSwReset = {0};
#endif




/*---------------------------------------------------------------------------------------
  function : cStorage()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cStorage::cStorage()
{
}

/*---------------------------------------------------------------------------------------
  function : ~cStorage()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cStorage::~cStorage()
{
}

/*---------------------------------------------------------------------------------------
  function : initStorage()
    @brief   :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cStorage::initStorage()
{
	printk(LOG_ALWAYS, F("initStorage()\n"));	

	cStorage::getStorage();	
	
	printk2(LOG_DEBUG, "g_stSwReset.u8SwReset = %u\n", g_stSwReset.u8SwReset);	
	
}

/*---------------------------------------------------------------------------------------
  function : setStorage()
    @brief   :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cStorage::setStorage(stSwReset_t* pSwReset)
{
	printk(LOG_ALWAYS, F("setStorage(0x%x, %u, %u, %u)\n"), g_stSwReset.u8SwReset
		,g_stSwReset.etc[0], g_stSwReset.etc[1], g_stSwReset.etc[2]);	

  kv_reset("/kv/");//comment out when retreiving saved data	
	kv_set("NVDATA", (uint8_t*)pSwReset, sizeof(stSwReset_t), 0);	
}

/*---------------------------------------------------------------------------------------
  function : getStorage()
    @brief   :
    @param
    @return
----------------------------------------------------------------------------------------*/
stSwReset_t* cStorage::getStorage()
{
	char sBuf[50];
	
	kv_get("NVDATA", (uint8_t*)&g_stSwReset, sizeof(stSwReset_t), 0);
	
	sprintf(sBuf, "getStorage() 0x%x, %u, %u, %u", g_stSwReset.u8SwReset, g_stSwReset.etc[0]
		       , g_stSwReset.etc[1], g_stSwReset.etc[2]);
	Serial.println(sBuf);

	return &g_stSwReset;
}

