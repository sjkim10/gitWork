/*****************************************************************************************
packet.h
~~~~~~~~~~~~~~~~~~~~~~~~~
	@ Project : AirQuant
	@ Date : 2024-02-19
	@ Author : Piquant
*****************************************************************************************/

#ifndef _PACKET_TEST_H_
#define _PACKET_TEST_H_

//#include <esp_attr.h>

#include "define.h"
#include "TemplateSingleTon.h"


/*--------------------------------------------------------------------------------------*/
/*                DEFINITION                                                            */
/*--------------------------------------------------------------------------------------*/
#define RESET_ESP32 0
#define RESET_ALL 1
#define FACTORY_RESET 2

/*--------------------------------------------------------------------------------------*/
/* (1) UART                                                                             */
/*--------------------------------------------------------------------------------------*/

#define SIZE_SENSOR_PACKET_DATA 800
#define START_DATA_PC 3


/*--------------------------------------------------------------------------------------*/
/* (2) ESP32 <--> PC                                                                    */
/*--------------------------------------------------------------------------------------*/

/* Command List : NANo33 BLE <--> PC */
#define PCCMD_ENTER_CFG       "EC"  //0
#define PCCMD_GET_DATA        "GD"  //1
#define PCCMD_SET_DATA        "SD"  //2
#define PCCMD_FORCE_RECAL     "FR"  //3
#define PCCMD_GET_CONFIG      "GC"  //4
#define PCCMD_SET_CONFIG      "SC"  //5
#define PCCMD_DO_RESET        "DR"  //6
#define PCCMD_GET_STATUS      "GS"  //7
#define PCCMD_GET_OP          "OP"  //8
#define PCCMD_RECONNECT_BLE   "RB"  //9
#define PCCMD_DEEP_SLEEP      "DS"  //10
#define PCCMD_CO2_CONTROL     "CC"  //11

#define PCCMD_TEST_MODE       "TM"  //12


/* Command List : ESP32 <--> SAMD */
enum
{
   CMD_ENTER_CFG = 1,  // 1
   CMD_GET_DATA,       // 2,
   CMD_SET_DATA,       // 3,
   CMD_FORCE_RECAL,    // 4 
   CMD_GET_CFG,        // 5,   
   CMD_SET_CFG,        // 6,
   CMD_SET_RESET,      // 7,
   CMD_GET_STATUS,     // 8,
   CMD_GET_OP,         // 9,
   CMD_RECONNECT_BLE,  // 10,
   CMD_DEEP_SLEEP,     // 11,
	 CMD_CO2_CONTROL,     // 12, 
   CMD_DO_TESTMODE,    // 13,

   CMD_MAX
};

/*--------------------------------------------------------------------------------------*/
/* (3) ESP32 <--> PC                                                                    */
/*--------------------------------------------------------------------------------------*/
/* Packet Response */
enum
{
   PKT_OK = 0,
   PKT_ERROR_STXETX,
   PKT_ERROR_CMD,
   PKT_ERROR_LENGTH,
   PKT_ERROR_CRC16,
   PKT_ERROR_MAX
};

/* Item : Arg[0] */
enum
{
   ITEM_RESET = 1,
   ITEM_FRESET,            // 2,
   ITEM_GET_ID,            // 3,
   ITEM_GET_STATUS,        // 4,
   ITEM_GET_CONFIG,        // 5,
   ITEM_SET_CONFIG,        // 6,
   ITEM_SET_DEFAULT_NET,   // 7,
   ITEM_SET_CLEAR_EEPROM,  // 8,
   ITEM_SHOW_PKT,          // 9,
   ITEM_GET_SENSOR_TIME,   // 10,
   ITEM_SET_REPORT_TIMER,  // 11,    
   ITEM_SET_TIMER_SAMD,    // 12, 
   ITEM_SET_TIMESTAMP,     // 13,
   ITEM_SCAN_I2C,          // 14,
   ITEM_DO_DIAG,           // 15,
   ITEM_DATA_START,        // 16,
   ITEM_SAVE_OFFSET_SLOP,  // 17,
   ITEM_GET_OFFSET_SLOP,   // 18,   
   TEST_SETTING_ITEM,      // 19,
   ITEM_TEST_NVS,          // 20,
   ITEM_GET_SENSOR_DATA,   // 21,
   ITEM_SET_CLEAR_NVS,     // 22,
   ITEM_SET_OFFSET_SLOPE,  // 23, 
   TTT_ITEM                // 24
};

enum
{
  ITEM_SEND_HEADINFO, // 0,
  ITEM_SEND_BIN,		 // 1,
  ITEM_FINISH_BIN,	 // 2,
  ITEM_RETRY_BIN, 	 // 3,	
  ITEM_RESET_SAMD,	 // 4
  ITEM_NO_UPDATE, 	 // 5
  ITEM_ERROR_NO_RETRY,// 6
};


/* CO2 */
enum
{
	SET_RECAL_SCD = 1,
	GET_RECAL_SCD,
	START_SENSING_SCD,
	STOP_SENSING_SCD,
	SET_AUTO_CAL,
	GET_AUTO_CAL
};

enum
{
   NVS_SETTING = 1,
   NVS_SENSOR_DATA = 2,
#if 1
   NVS_SET_DATA = 4,
#else
   NVS_COUNT_DATA = 4,
   NVS_SPECIFIC_DATA = 8   
#endif   
};

enum
{
   ERASE_DATA_NVS = 0,
   SET_DATA_NVS
};

enum
{
	READ_MODE = 0,
	WRITE_MODE,
	ERASE_MODE
};

/* packet head */
enum
{
   STX1_BIT = 0,
   STX2_BIT = 1,
   STX3_BIT = 2,
   LLEN_BIT = 3,
   HLEN_BIT = 4,
   CMD_BIT  = 5,
   RST_BIT  = 6,
   ARG1_BIT = 7, /* Item */
   ARG1_BIT2 = 8, /* Item */   
   ARG2_BIT = 9, /* Index */
   ARG2_BIT2 = 10, /* Index */   
   ARG3_BIT = 11, /* Sub-Index */
   ARG3_BIT2 = 12, /* Sub-Index */   
   MAX_HEAD_BIT = 13
};

/* packet tail */
enum
{
   LCRC_BIT = 0,
   HCRC_BIT = 1,
   ETX_BIT1 = 2,
   ETX_BIT2 = 3,
   ETX_BIT3 = 4,
   CR_BIT   = 5,
   //LF_BIT = 6,
   MAX_TAIL_BIT = 6
};

#define LEN_ONLY_BEFORE_DATA MAX_HEAD_BIT
#define LEN_ONLY_AFTER_DATA MAX_TAIL_BIT

#define START_TM_ITEM 1

/* Result Type */
enum
{
   RST_OK = 0xAA,
   RST_ERR = 0xEE
};

/*--------------------------------------------------------------------------------------*/
/* Structure                                                                            */
/*--------------------------------------------------------------------------------------*/

typedef struct _stCmd_t
{
	uint8_t u8Rst;
   uint8_t u8Cmd;
   uint16_t u16Item;
   uint16_t u16Index;
   uint16_t u16Index2;
   uint8_t u8PreCmd;
   uint16_t u16PreIndex;
   uint8_t u8PreLoopStatus;
} stCmd_t;
extern stCmd_t g_stCmd;

typedef struct _stReset_t
{
   bool bFatoryReset;
   bool bResetAll;
   bool bSaveCfg;
} stReset_t;
extern stReset_t g_stReset;

typedef struct _stPacketTail_t
{
   uint16_t _CRC16;
   uint8_t _ETX[3];
   uint8_t _CR;
//   uint8_t _LF;
} __attribute__((packed)) stPacketTail_t;
extern stPacketTail_t g_stPacketTail;

// The buffer is allocated with the largest data size, and when data is transmitted,
// it is transmitted with a variable size depending on the purpose.
typedef struct _stPacket_t
{
   uint8_t _STX[3]; /* STX : 53 54 4d */
   uint16_t _LEN;
   uint8_t _CMD;
   uint8_t _RST;
   uint16_t _ARG[SIZE_PACKET_ARGUMENT];
   uint8_t _DATA[1]; /* 1 byte */
} __attribute__((packed)) stPacket_t;


extern const unsigned char P_STX[4];
extern const unsigned char P_ETX[4];
extern const unsigned char P2_STX[4];

extern const unsigned char CR_CHANGE[3];
extern const unsigned char LF_CHANGE[3];

extern const unsigned char CR;
extern const unsigned char LF;

extern const uint8_t uRESPONSE_OK[];
extern const uint8_t uRESPONSE_FAIL[];



/*****************************************************************************************
 *                              cEthernetDev  Class
 *****************************************************************************************/
class cPacket : public TemplateSingleton<cPacket>
{
public:
	cPacket();
	virtual ~cPacket();

	uint8_t uDoCommandPC(char *pCmd, stCmd_t *pstCmd, uint8_t *pOnlyData, uint16_t u16SizeOnlyData);
	uint8_t uProcessPacketMsgPC(bool bSoftAp, uint8_t *pRxPacket, uint16_t u16RxPacketBytes);
	uint8_t uProcessStringMsgPC(char *pRxPacket, uint16_t u16RxPacketBytes);

	bool bVerifyPacketMsg(uint8_t *pRxPacket, uint16_t u16RxPacketBytes, uint16_t *u16NewRxPacketBytes);
	bool bCheckRxPakcet(uint8_t *pRxPacket, uint16_t u16RxPacketBytes);

	static uint16_t wGenCrc16(const uint8_t *pBuf, int nSize);
	// uint8_t *pMakeTxPacket(uint8_t uCommand, uint8_t uRst, uint16_t *pArg, uint8_t *pData, uint16_t wSizePacket);
	uint8_t *pMakeTxPacket(uint8_t uCommand, uint8_t uRst, uint16_t *pArg, uint8_t *pData, uint16_t wSizePacket, uint16_t *wNewSize);
	uint8_t *pRemoveAddCharacters(uint8_t *pData, uint16_t wSizePacket, uint16_t *wNewSize);

private:
	uint16_t m_u16ARGUMENT[SIZE_PACKET_ARGUMENT];
};

#endif /* _PACKET_TEST_H_ */
