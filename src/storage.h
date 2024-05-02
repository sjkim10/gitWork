/*****************************************************************************************
storage.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-03-05
@ Author : Piquant
*****************************************************************************************/

#ifndef _MY_STORAGE_TEST_H_
#define _MY_STORAGE_TEST_H_

#include "define.h"
#include "TemplateSingleTon.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

#define SW_RESET   0xAA


typedef struct _stSwReset_t
{
	uint8_t u8SwReset;
	uint8_t etc[3];
#if 0	
} __attribute__ ((section (".noinit"))) stSwReset_t ;
extern volatile stSwReset_t g_stSwReset;

#else
} stSwReset_t ;
extern stSwReset_t g_stSwReset;
#endif

/*****************************************************************************************
                        cTest  Class
 *****************************************************************************************/
class cStorage : public TemplateSingleton<cStorage>
{
public:
   cStorage();
   virtual ~cStorage();

  void initStorage();
	static void setStorage(stSwReset_t* pSwReset);
	static stSwReset_t* getStorage();
};

#endif /* _MY_STORAGE_TEST_H_ */
