/*****************************************************************************************
utils.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _UTILS_TEST_H_
#define _UTILS_TEST_H_

//#include <esp_attr.h>

#include "define.h"
#include "TemplateSingleTon.h"

/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/

#define UNUSED(v) (void)(v)

#define MIN2(a, b) (((a) < (b)) ? (a) : (b))
#define MAX2(a, b) (((a) > (b)) ? (a) : (b))

#define MAKEWORD(low, high) ((WORD)(((uint8_t)(low)) | ((WORD)((uint8_t)(high))) << 8))
#define LOWORD(l) ((WORD)(l))
#define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w) ((BYTE)(w))
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))
/* 0x12345678 -> 0x78563412 */
#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))


/* int16_t x, int16_t y */
#define nAVERAGE (x, y)(((x) + (y)) >> 1) /* float(x), double (x) */
/* uint16_t x, uint16_t y */
#define uAVERAGE (x, y)(((x) & (y)) + (((x) ^ (y)) >> 1)) /* float(x), double (x) */

#define IsNAN(a) ( ((a) != (a)) ? (1) : (0)) 
#define IsINF(a) ( (!IsNAN(a) && IsNAN(a - a)) ? (1) : (0)) 
#define CheckNAN(a) ( (!IsNAN(a) && !IsINF(a) && (a>0)) ? (a) : (0)) 

#if 0
if (x != x)
{
    // x is NaN
}
#endif

#define NO_ARRAY_PARSE 20
#define NO_SUB_ARRAY 32
typedef struct _stParse_t
{
   char d[NO_ARRAY_PARSE][NO_SUB_ARRAY];
   BYTE cmd;
   BYTE noData;
} stParse_t;

// extern stParse_t g_stPcParse;

/*--------------------------------------------------------------------------------------*/
/*                Extern                                                                */
/*--------------------------------------------------------------------------------------*/

/*****************************************************************************************
 *                              cUtils  Class
 *****************************************************************************************/
class cUtils : public TemplateSingleton<cUtils>
{
public:
   cUtils();
   virtual ~cUtils();

   static uint8_t strtokenizer(char *pData, const char *pT);
   static inline WORD bswap16(WORD a);
   static inline WORD bytes2word(bool bReverse, BYTE *pByte);
   static inline void word2bytes(WORD wData, BYTE *b);
   static DWORD bytes2dword(BYTE *pByte, BYTE nNum);
   static void dword2bytes(DWORD dwData, BYTE *b);
   static uint16_t average(uint16_t x, uint16_t y);

public:
   static stParse_t m_stPcParse;
};

#endif /* _UTILS_TEST_H_ */
