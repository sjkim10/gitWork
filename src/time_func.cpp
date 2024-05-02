/*****************************************************************************************
  time_func.cpp
 -------------------------------------
  @ Project : AirQuant
	@ Date : 2024-02-24
  @ Author : Piquant
*****************************************************************************************/
#include <Arduino.h> // using Arduino development platform

#include <stdio.h>
#include <time.h>

#include "time_func.h"
#include "timer.h"
#include "config.h"
#include "print.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
//ESP32Time cTimeFunc::m_rtc;
//struct tm g_timeinfo;

// ESP32Time g_rtc;
// struct tm g_timeinfo;
char g_sTimeStamp[100];

/*---------------------------------------------------------------------------------------
  function : cTimeFunc()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cTimeFunc::cTimeFunc()
{
   m_bGotTime = false;
}

/*---------------------------------------------------------------------------------------
  function : ~cTimeFunc()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
cTimeFunc::~cTimeFunc()
{
}

/*---------------------------------------------------------------------------------------
  function : tmMakeLocalTime()
    @brief    : time_t representing the number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC (i.e., a unix timestamp).
    @param
    @return
----------------------------------------------------------------------------------------*/
time_t cTimeFunc::getUTCtime()
{
#ifdef USE_AWS
   time_t tNowTime;
   time(&tNowTime);
#else
   unsigned long tNowTime = 112233;
#endif

   if (m_bGotTime == false)
   {
      printk(LOG_DEBUG, F("<Warning> getUTCtime(%u) m_bGotTime = false\n"));
      return 0;
   }
   else
   {
#ifdef USE_AWS 
      time(&tNowTime);
#endif
#if 0
      char sbuf[50] = {0};
      sprintf(sbuf, "%10ld", tNowTime);
#endif
      return tNowTime;
   }
}

/*---------------------------------------------------------------------------------------
  function : tmMakeLocalTime()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
time_t cTimeFunc::tmMakeLocalTime(int YYYY, int MM, int DD, int hh, int mi, int ss)
{
   struct tm st_tm;
   st_tm.tm_year = YYYY - 1900;
   st_tm.tm_mon = MM - 1;
   st_tm.tm_mday = DD;
   st_tm.tm_hour = hh;
   st_tm.tm_min = mi;
   st_tm.tm_sec = ss;
   return mktime(&st_tm);
}

/*---------------------------------------------------------------------------------------
  function : tmMakeUTCTime()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
time_t cTimeFunc::tmMakeUTCTime(int YYYY, int MM, int DD, int hh, int mi, int ss)
{
#if 0   
   struct tm st_tm;
   st_tm.tm_year = YYYY - 1900;
   st_tm.tm_mon = MM - 1;
   st_tm.tm_mday = DD;
   st_tm.tm_hour = hh;
   st_tm.tm_min = mi;
   st_tm.tm_sec = ss;
   return _mkgmtime(&st_tm);
#endif
   return 0;
}

/*---------------------------------------------------------------------------------------
  function : sMakeLocalTime()
    @brief    : time_t and ctime
             time_t t1;
             time(&t1);
             char* str = ctime(&t1);
    @param
    @return
----------------------------------------------------------------------------------------*/
char *cTimeFunc::sMakeLocalTime(time_t t1)
{
   static char *_str = ctime(&t1);
   printk(LOG_ALWAYS, F("ctime : %s\n"), _str);

   return _str;
}

/*---------------------------------------------------------------------------------------
  function : sMakeLocalTime()
    @brief    : struct tm and asctime
    @param
    @return
----------------------------------------------------------------------------------------*/
char *cTimeFunc::sMakeLocalTime(struct tm *pTm)
{
   static char *_str2 = asctime(pTm);
   printk(LOG_ALWAYS, F("asctime : %s\n"), _str2);

   return _str2;
}

/*---------------------------------------------------------------------------------------
  function : tmTest()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
void cTimeFunc::tmTest()
{
   char sbuf[50] = {0};

   // time_t 값으로 변환할 날짜
   int year = 2020;
   int mon = 12;
   int day = 25;
   int hour = 0;
   int min = 0;
   int sec = 0;

   time_t localTime = tmMakeLocalTime(year, mon, day, hour, min, sec);
   sprintf(sbuf, "%10ld", localTime);
   printk(LOG_ALWAYS, F("2020 x-mas local time_t : %s\n"), sbuf);

   time_t utcTime = tmMakeUTCTime(year, mon, day, hour, min, sec);
   sprintf(sbuf, "%10ld", utcTime);
   printk(LOG_ALWAYS, F("2020 x-mas UTC time_t : %s\n"), sbuf);

   //현재부터 2020년 크리스마스 까지 남은 초
   time_t curTime;
   time(&curTime);
   sprintf(sbuf, "%10ld", localTime - curTime);
   printk(LOG_ALWAYS, F("time remaining until 2020 x-mas : %s\n"), sbuf);

   // 2020년 크리스마스 이후인지 검사
   if (curTime >= localTime)
   {
      printk(LOG_ALWAYS, F("Merry X-mas"));
   }
}

/*---------------------------------------------------------------------------------------
  function : setClock()
    @brief    :
    @param
    @return
----------------------------------------------------------------------------------------*/
bool cTimeFunc::bSetClock()
{
#if 0
   struct tm timeinfo;
   long lms;
   static bool _bSetTime = false;
   const long gmtOffset_sec = 3600 * 9;
   const int daylightOffset_sec = 0;

   if (_bSetTime == true)
   {
      return true;
   }

//   configTime(gmtOffset_sec, daylightOffset_sec, "asia.pool.ntp.org");
   configTime(gmtOffset_sec, daylightOffset_sec, SERVER_CURRENT_TIME);
   if (getLocalTime(&timeinfo) == true)
   {
      int year = timeinfo.tm_year + 1900;
      int month = timeinfo.tm_mon + 1;
      int day = timeinfo.tm_mday;
      int hour = timeinfo.tm_hour;
      int min = timeinfo.tm_min;
      int sec = timeinfo.tm_sec;

      m_rtc.setTime(sec, min, hour, day, month, year);
      _bSetTime = true;

      g_timeinfo = m_rtc.getTimeStruct();
      lms = m_rtc.getMillis();
      sprintf(g_sTimeStamp, "%04d-%02d-%02d %02d:%02d:%02d:%ld", g_timeinfo.tm_year + 1900, g_timeinfo.tm_mon + 1, g_timeinfo.tm_mday, g_timeinfo.tm_hour, g_timeinfo.tm_min, g_timeinfo.tm_sec, lms);

      printk(LOG_ALWAYS, F("%s\n"), "-------------------------------------------------------");
      printk(LOG_ALWAYS, F("[bSetClock] g_sTimeStamp : %s\n"), g_sTimeStamp);
      printk(LOG_ALWAYS, F("%s\n"), "-------------------------------------------------------");

      return true;
   }
   else
   {
      printk(LOG_ERROR, F("---------- <Error>[bSetClock]\n"));
   }
   return false;
#endif

	return true;
}
