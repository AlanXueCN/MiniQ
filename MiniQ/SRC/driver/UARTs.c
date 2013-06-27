/* UARTs.C file

���ܣ�ʵ��	STM32-SDK �������ϵ� UART1-RS232 �ӿڲ���

---------Ӳ���ϵ���������:----------
RS232�ӿڣ�
RS232TXD  -->  PA9  (UART1-TXD)
RS232RXD  -->  PA10 (UART1-RXD)
------------------------------------
 */

#include "UARTs.h"
#include "../algorithm/control.h"
#include "extern_variable.h"
u8 TxBuffer[258];
u8 TxCounter=0;
u8 count=0; 
u8 start_mark[2]={0xff,0xaa};
u8 recvStatus=0;
u8 recvCmd=0;
u8 recvLength=0;
u8 recvPos=0;
u8 recvData[8]={0,0,0,0,0,0,0,0};
extern char baseThr;

extern PID PID_RP;
void parseCmd(u8 cmd, u8 len);
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void Initial_UART1(u32 baudrate)
*��������:		��ʼ��STM32-SDK�������ϵ�RS232�ӿ�
���������
		u32 baudrate   ����RS232���ڵĲ�����
���������û��	
*******************************************************************************/
void Initial_UART1(u32 baudrate)
{
USART_InitTypeDef USART_InitStructure;
USART_ClockInitTypeDef  USART_ClockInitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 |RCC_APB2Periph_USART1, ENABLE  );

USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;			// ʱ�ӵ͵�ƽ�
USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;				// ʱ�ӵ͵�ƽ
USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;				// ʱ�ӵڶ������ؽ������ݲ���
USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;		// ���һλ���ݵ�ʱ�����岻��SCLK���
/* Configure the USART1 synchronous paramters */
USART_ClockInit(USART1, &USART_ClockInitStructure);					// ʱ�Ӳ�����ʼ������
																	 
USART_InitStructure.USART_BaudRate = 115200;						  // ������Ϊ��115200
USART_InitStructure.USART_WordLength = USART_WordLength_8b;			  // 8λ����
USART_InitStructure.USART_StopBits = USART_StopBits_1;				  // ��֡��β����1��ֹͣλ
USART_InitStructure.USART_Parity = USART_Parity_No ;				  // ��żʧ��
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// Ӳ��������ʧ��

USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		  // ����ʹ��+����ʹ��
/* Configure USART1 basic and asynchronous paramters */
USART_Init(USART1, &USART_InitStructure);
    
  /* Enable USART1 */
USART_ClearFlag(USART1, USART_IT_RXNE); 			//���жϣ�����һ�����жϺ����������ж�
USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);		//ʹ��USART1�ж�Դ
USART_Cmd(USART1, ENABLE);							//USART1�ܿ��أ����� 
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART1_Put_Char(unsigned char DataToSend)
*��������:		RS232����һ���ֽ�
���������
		unsigned char DataToSend   Ҫ���͵��ֽ�����
���������û��	
*******************************************************************************/
void UART1_Put_Char(unsigned char DataToSend)
{
	TxBuffer[count++] = DataToSend;  
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);   
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 UART1_Get_Char(void)
*��������:		RS232����һ���ֽ�  һֱ�ȴ���ֱ��UART1���յ�һ���ֽڵ����ݡ�
���������		 û��
���������       UART1���յ�������	
*******************************************************************************/
u8 UART1_Get_Char(void)
{
	while (!(USART1->SR & USART_FLAG_RXNE));
	return(USART_ReceiveData(USART1));
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART1_Put_String(unsigned char *Str)
*��������:		RS232�����ַ���
���������
		unsigned char *Str   Ҫ���͵��ַ���
���������û��	
*******************************************************************************/
void UART1_Put_String(unsigned char *Str)
{
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str){
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r')UART1_Put_Char(0x0d);
		else if(*Str=='\n')UART1_Put_Char(0x0a);
			else UART1_Put_Char(*Str);
	//�ȴ��������.
  	//while (!(USART1->SR & USART_FLAG_TXE));
	//ָ��++ ָ����һ���ֽ�.
	Str++;
	}

}


//------------------------------------------------------
void USART1_IRQHandler(void)
{

	char RX_dat;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1,   USART_IT_RXNE);

		RX_dat=USART_ReceiveData(USART1);// & 0x7F;
    //status = RX_dat;
    if(recvStatus==0)
    {
      if(RX_dat==0xff) recvStatus=1;
    }
    else if(recvStatus==1)
    {
      if(RX_dat==0xaa) recvStatus=2;
      else recvStatus=0;
    }
    else if(recvStatus==2)
    {
      recvCmd=RX_dat;
      recvStatus=3;
    }
    else if(recvStatus==3)
    {
      recvLength=RX_dat;
      recvStatus=4;
    }
    else if(recvStatus==4)
    {
      if(recvPos<recvLength)
      {
        recvData[recvPos]=RX_dat;
        ++recvPos;
      }
      if(recvPos==recvLength) 
      {
        parseCmd(recvCmd,recvLength);
        recvPos=0;
        recvStatus=0;
      }
    }
		//USART_SendData(USART1, RX_dat);

		//while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
	}
  else if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {
    
    USART_SendData(USART1, TxBuffer[TxCounter++]);                    

    /* Clear the USART1 transmit interrupt */
    USART_ClearITPendingBit(USART1, USART_IT_TXE); 

    if(TxCounter == count)
    {
      /* Disable the USART1 Transmit interrupt */
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    }    

  }
  


}


void parseCmd(u8 cmd, u8 len)
{
  char* temp;
  switch (cmd)
  {
    case 0x01:
      baseThr=recvData[0];
      break;
    case 0x02:
      temp=(char*)(&PID_RP.P);
      temp[0] = recvData[0];
      temp[1] = recvData[1];
      temp[2] = recvData[2];
      temp[3] = recvData[3];
      break;
    case 0x03:
      temp=(char*)(&PID_RP.D);
      temp[0] = recvData[0];
      temp[1] = recvData[1];
      temp[2] = recvData[2];
      temp[3] = recvData[3];
    break;
    case 0x04:
      temp=(char*)(&PID_RP.I);
      temp[0] = recvData[0];
      temp[1] = recvData[1];
      temp[2] = recvData[2];
      temp[3] = recvData[3];
    break;
    case 0x05:
      temp=(char*)(&EXP_ANGLE.X);
      temp[0] = recvData[0];
      temp[1] = recvData[1];
      temp[2] = recvData[2];
      temp[3] = recvData[3];
    break;
    case 0x06:
      temp=(char*)(&EXP_ANGLE.Y);
      temp[0] = recvData[0];
      temp[1] = recvData[1];
      temp[2] = recvData[2];
      temp[3] = recvData[3];
    break;
    case 0x07:
      temp=(char*)(&EXP_ANGLE.Z);
      temp[0] = recvData[0];
      temp[1] = recvData[1];
      temp[2] = recvData[2];
      temp[3] = recvData[3];
    break;
  }
  
}

//------------------End of File----------------------------
