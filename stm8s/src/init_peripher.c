/**********************************************************************************

File_name:      init_peripher.c
Version:		   0.0
Revised:        $Date:2014-7-  ; $
Description:    the main file of this project.
Notes:          This version targets the stm8
Editor:		    Mr.Kong

**********************************************************************************/


/*================================================================================
@ Include files
*/
#include "init_peripher.h"


/*================================================================================
@ Global variable
*/


/*================================================================================
@ All functions  as follow
*/

/*********************************************************************************
Function:  void Delay_Ms(u16 z)
Description:
delay 1ms @  8MHz

Input:        ByteData you want to send.
Output:       None
Return:       None
Others:
*********************************************************************************/
void Delay_Ms(u16 z)
{
  u16 x,y;
  for(x=z;x>0;x--)
  { for(y=1600;y>0;y--);}


}

/*********************************************************************************
**���ƣ�void EEPROM_WriteByte(uchar EEPROM_Data,uint EEPROM_Add)
**
**���ܣ���EEPROM д��һ�ֽ�
**
**˵�����洢һ�ֽ����� 0-639
**
**ʱ�䣺2013.5.6
**********************************************************************************/
void EEPROM_WriteByte(uchar EEPROM_Data,u16 EEPROM_Add)
{
  uchar *Address;
  Address=(uchar *)(EEPROM_Add+0x4000);
  do
  {
    FLASH_DUKR=0xae; // д���һ����Կ
    FLASH_DUKR=0x56; // д��ڶ�����Կ
  }
  while((FLASH_IAPSR&0x08)==0); // ������δ�ɹ�������������
  *Address =EEPROM_Data;     // д���һ���ֽ�
  while((FLASH_IAPSR&0x04)==0); // �ȴ�д�����ɹ�
}




/*********************************************************************************
**���ƣ�uchar EEPROM_ReadByte(uint EEPROM_Add)
**
**���ܣ���EEPROM ��ȡһ�ֽ�
**
**˵������ȡһ�ֽ�����
**
**ʱ�䣺2013.5.6
*********************************************************************************/
uchar EEPROM_ReadByte(u16 EEPROM_Add)
{
  uchar EEPROM_Data;
  EEPROM_Data = *((uchar*) (EEPROM_Add+0x4000));

  return EEPROM_Data;
}


/********************************************************************************
Function:  void UART1_SendBytE
Description:
send a byte by UART1
Input:        ByteData you want to send.
Output:       None
Return:       None
Others:
*********************************************************************************/
void UART1_SendByte(uchar ByteData)
{

   while(!UART1_SR_TXE);
   UART1_DR=ByteData;     //��USART1����һ�ֽ�����

   while(!UART1_SR_TC);   //�Ƿ������

}

/********************************************************************************
Function:       TIM1_PWM_Init
Description:
configurate PWM generator
Input:        CarrierFrq  you set
Output:       None
Return:       None
Others:
*********************************************************************************/
void TIM1_PWM_Init(u8 CarrierFrq)

{

  u16  TIM1_ARR;

  TIM1_CCMR1=TIM1_CCMR1|0x70;
  TIM1_CCER1=TIM1_CCER1|0x03;

  TIM1_ARR  = ( 16000 / CarrierFrq ) -1 ;// calculate the timer1 count.
  TIM1_ARRH =( TIM1_ARR >> 8) & 0xFF;//
  TIM1_ARRL = TIM1_ARR & 0XFF;


  TIM1_ARR = (TIM1_ARR +1 )/ 3 -1 ;//calculate the timer1 compare data.
  TIM1_CCR1H = (TIM1_ARR >> 8) & 0xFF;//
  TIM1_CCR1L = TIM1_ARR & 0xFF;

  TIM1_PSCRH = 0x00;
  TIM1_PSCRL = 0x00;
  //TIM1_CCMR1_OC1M = 100;
  // TIM1_CR1=TIM1_CR1|0X01;
  TIM1_BKR=0x80;
  DISABLE_TIM1_PWM;



}
/********************************************************************************
Function: void SendFrame(u8 *data,u8 Length)
Description: send a frame of data by UART1

Input:     u8 *data    pointer  of a series of data you want to send.
u8 Length  the number of this series of

data.
Output:       None
Return:       None
Others:
*********************************************************************************/
void SendFrame(u8 *data,u8 Length)
{

  while( Length>0 )
  {

    UART1_SendByte(*data);
    data++;
    Length--;

  }
}

/********************************************************************************
**���ƣ�void TIM4_Init(void)
**
**���ܣ���ʱ��4 ��ʼ��
**
**˵������ʱ��4 ����ģ�⴮��  ������Ϊ9600
**
**ʱ�䣺2013.04.26
*********************************************************************************/
void TIM4_Init(void)
{
  TIM4_IER=0x00; // ��ֹ�ж�
  TIM4_EGR=0x01; // ������������¼� // ���ÿ�������������ʱ��

  TIM4_PSCR = 0x00;//0.0625us

  TIM4_ARR  = 0xEF; // 15us
  TIM4_CNTR = 0XEF;

  //TIM4_CR1=0x01; // b0=1,�������������
  TIM4_IER=0x01; // �����ж�
  ENABLE_TIM4  ;


}
/********************************************************************************
**���ƣ�void IWDG_Init(void)
**
**���ܣ��������Ź���ʼ��
**
**˵����
**
**ʱ�䣺2013.09.27
*********************************************************************************/
void IWDG_Init(void)
{

  IWDG_KR=0xcc;
  IWDG_KR=0x55;
  IWDG_PR=0x06;
  IWDG_RLR=0xff;
  IWDG_KR=0xaa;

}




/********************************************************************************
Function:  All_Iint
Description:  external device will be configurated in this sector.
Input:        None
Output:       None
Return:       None
Others:
*********************************************************************************/
void All_Init(void)
{


  /***************MCU clock config*********/
  CLK_ECKR=0x03;//�ⲿʱ�ӼĴ��� �ⲿʱ��׼���������ⲿʱ�ӿ�
  CLK_CKDIVR = 0x00;
  CLK_SWCR=0x02;//�л����ƼĴ��� ʹ���л�����
  CLK_SWR=0xB4;//��ʱ���л��Ĵ��� ѡ��HSEΪ��ʱ��Դ


  while (!(CLK_SWCR & 0x08));
  CLK_CSSR=0x05;//�л��жϿ�����ʱ�Ӱ�ȫϵͳʹ��
  CLK_ICKR_LSIEN = 1; //�������Ź�ʱ��
  // IWDG_Init();//in


  /* ALARM_INDICATOR  PA6 push_pull output*/

  PA_DDR_DDR6 = 1;
  PA_CR1_C16  = 1;
  PA_CR2_C26  = 0; //LED

  /*RUNNING_INDICATOR PD3 push_pull output*/

  PD_DDR_DDR2 = 1;
  PD_CR1_C12  = 1;
  PD_CR2_C22  = 0; //LED

  /*STATUS_INDICATOR PC1 PC2 PC3 PC4 push_pull output*/

  PC_DDR_DDR1 = 1;
  PC_CR1_C11  = 1;
  PC_CR2_C21  = 0; //INDICATOR_1

  PC_DDR_DDR2 = 1;
  PC_CR1_C12  = 1;
  PC_CR2_C22  = 0; //INDICATOR_2

  PC_DDR_DDR3 = 1;
  PC_CR1_C13  = 1;
  PC_CR2_C23  = 0; //INDICATOR_3

  PC_DDR_DDR4 = 1;
  PC_CR1_C14  = 1;
  PC_CR2_C24  = 0; //INDICATOR_4


  /*******serial_port_config************/


  //UART1
     UART1_BRR2= 0x02;
     UART1_BRR1= 0x68;
     UART1_CR2 = 0x2c;//������գ����ͣ��������ж�
  //UART3
     UART3_BRR2= 0x02;
     UART3_BRR1= 0x68;
     UART3_CR2 = 0x2c;//������գ����ͣ��������ж�

   //UART1_CR2=0x0c;//�رս����ж�

  /*************TIM2_config************/

  //��Ҫ����10ms��ʱ
  TIM2_IER = 0x00; // ��ֹ�ж�
  TIM2_EGR = 0x01; // ������������¼� // ���ÿ�������������ʱ��

  TIM2_PSCR = 0x08;//������ʱ��=��ʱ��/2^8�൱�ڼ���������Ϊ16us

  TIM2_ARRH = 0x02; // �趨��װ��ʱ�ļĴ���ֵ��
  TIM2_ARRL = 0x70;
  TIM2_CNTRH = 0x02;// �趨�������ĳ�ֵ
  TIM2_CNTRL = 0x70;// ��ʱ����=(ARR+1)*16uS=10mS;
  TIM2_CR1 = 0x01; // b0=1,�������������
  TIM2_IER = 0x01; // �����ж�
  //   //TIM1_PWM_Init( CarrierFreqency);



  /*************TIM4_config***********/
  //��ҪΪ�������ݼ�ʱ��
  //   TIM4_CR1= 0x01;//ʹ�ܼ�����
  //
  //   TIM4_IER= 0x01;//�����ж�ʹ��
  //   TIM4_EGR= 0x01;
  //   TIM4_CNTR= 255;//������ֵ
  //   TIM4_ARR=  255;//�Զ���װ��ֵ
  //   TIM4_PSCR= 0x07;//128��Ƶֵ
  //   ENABLE_TIM4  ;

}


/*********************************************************************************
Function:  ModBus_CRC16
Description:
Calculate  ModBus_CRC16

Input:       uchar *puchMsg :   the buffer address
uchar crc_count :   length of data buffer

Output:       None
Return:       CRC
Others:	      dont care
*********************************************************************************/
u16 ModBus_CRC16(uchar *puchMsg,uchar crc_count)
 {
    uchar i ,j;
    uchar XORResult;
    u16 xorCRC = 0xA001; //����CRC�Ķ���ʽ
    u16 CRC = 0xFFFF;
    for ( i = 0; i <crc_count; i++)
     {
        CRC ^= puchMsg[i];
        for (j = 0; j < 8; j++)
         {
            XORResult = CRC & 1;
            CRC >>= 1;
            if (XORResult)
               CRC ^= xorCRC;
         }
     }

    return  CRC;
 }
/*@*****************************end of file*************************************@*/
