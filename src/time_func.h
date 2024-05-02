/*****************************************************************************************
time_func.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _MY_TUTC_TEST_H_
#define _MY_TUTC_TEST_H_

//#include <ESP32Time.h>
#include "sw_feature.h"
#include "TemplateSingleTon.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
//extern struct tm g_timeinfo;

/*****************************************************************************************
 *                                 cTimeFunc  Class
 *****************************************************************************************/
class cTimeFunc : public TemplateSingleton<cTimeFunc>
{
public:
   cTimeFunc();
   virtual ~cTimeFunc();

   time_t getUTCtime();
   time_t tmMakeLocalTime(int YYYY, int MM, int DD, int hh, int mi, int ss);
   time_t tmMakeUTCTime(int YYYY, int MM, int DD, int hh, int mi, int ss);
   char *sMakeLocalTime(time_t t1);
   char *sMakeLocalTime(struct tm *pTm);

   void tmTest();

   bool bSetClock();

public:
//   static ESP32Time m_rtc;

private:
   bool m_bGotTime;
};

#endif /* _MY_TUTC_TEST_H_ */
