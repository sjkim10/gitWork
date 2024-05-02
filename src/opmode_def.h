/*****************************************************************************************
opmode_def.h
~~~~~~~~~~~~~~~~~~~~~~~~~
@ Project : AirQuant
@ Date : 2024-02-24
@ Author : Piquant
*****************************************************************************************/

#ifndef _OPMODE_DEF_TEST_H_
#define _OPMODE_DEF_TEST_H_



/*****************************************************************************************
*                                Extern & Definition
*****************************************************************************************/

/* Operation Mode for tate Machine */
enum
{
   OP_INIT = 0,
   OP_IDLE = 1,
   OP_BLE = 2,
   OP_SLEEP = 3,
   OP_RESET = 4,
   OP_CONFIG = 5,
   OP_CO2 = 6,    
   OP_TEST = 7,
   OP_DUMMY = 8,
   
   OP_NONE = 9,
};

typedef struct _stDoOption_t
{
   bool bTestMode;
   bool bStartStop;
   uint8_t uSensor;
   uint8_t uRepeatTime;

} stDoOption_t;


#endif /* _OPMODE_DEF_TEST_H_ */

