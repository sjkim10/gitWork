/*****************************************************************************************
init.h
~~~~~~~~~~~~~~~~~~~~~~~~~
	@ Project : AirQuant
	@ Date : 2024-02-19
	@ Author : Piquant
*****************************************************************************************/

#ifndef _INIT_TEST_H_
#define _INIT_TEST_H_

#include "define.h"
#include "TemplateSingleTon.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/


typedef struct
{
   void *pBleBeacon;
   void *pTimer;
   void *pTmFunc;
   void *pTask;
   void *pUtils;
   void *pSystem;
	 void *pLed; 
	 void *pCo2; 	 
	 void *pSpi;
   void *pI2c;
   void *pGpio;
	 void *pPacket;
	 void *pOpMode;
	 void *pUart;	 
	 void *pTest;
} stInsatnce_t;

extern stInsatnce_t g_stInst;

 

/*****************************************************************************************
 *                              cInit  Class
 *****************************************************************************************/
class cInit : public TemplateSingleton<cInit>
{
public:
	cInit();
	virtual ~cInit();

	static bool bInit();

	void initDevice();
	static char *showVersion();
	void initTasks();
	void getinstance();

public:
	static bool m_u8LoopStatus;
};


#endif /* _INIT_TEST_H_ */
