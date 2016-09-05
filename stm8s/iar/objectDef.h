/************************************************************************************

File_name:      objectDef.h
Description:    the header file of  Model_c .
Editor:		      Mr.Kong

************************************************************************************/
#ifndef     __OBJECTDEF_H__
#define     __OBJECTDEF_H__

/*==================================================================================
@ Include files
*/
#include "includes.h"
#include "io_macros.h"


/*==================================================================================
@ Typedefs
*/
#define AUTO_RUN           0x00
#define FORCE_OEN          0x01
#define FORCE_CLOSE          -1

typedef struct _SLAVERS_STORAGE
{

  u8  address;          //��·��ַ
  u8  loadType;         //��������
  u8  attributed;       // 0x00: monophase ���ࣻTHREE-PHASE ����
  u8  openCloseTimes;   //���ش�����¼
  u32 openTime;         //����ʱ����ۼ�
  u8  hasMeter;         //���޵���ģ��

  s8  workMode;         //FORCE_OPEN ǿ��  FORCE_CLOSEǿ��  AUTO_RUN�Զ�
}SLAVERS_STORAGE;

typedef struct _ENERGY_METER
{
  u16  voltage;
  u16  current;
  u16  power;
  u32  totalEnergy;
  u8   factor;
  u16  frequency;
  u8   energyErr;
}ENERGY_METER, *pENERGY_METER;


typedef struct _SALVERS
{
  SLAVERS_STORAGE salverStorage;
  ENERGY_METER energyMeter;
}SLAVERS, *pSLAVERS;


//system information define
typedef struct _SYSTEM_STORAGE
{
  u8 systemAdress;
  u8 slaversNumber;
}SYSTEM_STORAGE;

typedef struct _SYSTEM
{
  SYSTEM_STORAGE systemStorage;
  OS_EVENT *SemUART1;

  u8 UART1RxCount;
  u8 UART1TxAddress;
  u8 UART1RxBuf[18];

  OS_EVENT *SemFrameStart;
  OS_EVENT *SemFrameEnd;
  u8 FrameRxCount;
  u8 FrameRxBuf[20];

}SYSTEM,*pSYSTEM;
/*==================================================================================
@ Constants and defines
*/









#endif
/*@*****************************end of file**************************************@*/


