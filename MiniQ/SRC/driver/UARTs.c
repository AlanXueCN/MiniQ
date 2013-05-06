/* UARTs.C file

���ܣ�ʵ��	STM32-SDK �������ϵ� UART1-RS232 �ӿڲ���

---------Ӳ���ϵ���������:----------
RS232�ӿڣ�
RS232TXD  -->  PA9  (UART1-TXD)
RS232RXD  -->  PA10 (UART1-RXD)
------------------------------------
 */

#include "UARTs.h"

u8 TxBuffer[258];
u8 TxCounter=0;
u8 count=0; 


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void Initial_UART1(u32 baudrate)
*��������:		��ʼ��STM32-SDK�������ϵ�RS232�ӿ�
���������
		u32 baudrate   ����RS232���ڵĲ�����
���������û��	
*******************************************************************************/
void Initial_UART1(u32 baudrate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  /* ʹ�� UART1 ģ���ʱ��  ʹ�� UART1��Ӧ�����Ŷ˿�PA��ʱ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

  /* ����UART1 �ķ�������
	����PA9 Ϊ�������  ˢ��Ƶ��50MHz
	*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  /* 
	����UART1 �Ľ�������
	����PA10Ϊ�������� 
  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* 
  UART1������:
  1.������Ϊ���ó���ָ�������� baudrate;
  2. 8λ����			  USART_WordLength_8b;
  3.һ��ֹͣλ			  USART_StopBits_1;
  4. ����żЧ��			  USART_Parity_No ;
  5.��ʹ��Ӳ��������	  USART_HardwareFlowControl_None;
  6.ʹ�ܷ��ͺͽ��չ���	  USART_Mode_Rx | USART_Mode_Tx;
  */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//Ӧ�����õ�UART1
	USART_Init(USART1, &USART_InitStructure); 
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//����UART1
  USART_Cmd(USART1, ENABLE);
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
	//��Ҫ���͵��ֽ�д��UART1�ķ��ͻ�����
	//USART_SendData(USART1, (unsigned char) DataToSend);
	//�ȴ��������
  	//while (!(USART1->SR & USART_FLAG_TXE));

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
/*
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str){
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r')USART_SendData(USART1, 0x0d);
		else if(*Str=='\n')USART_SendData(USART1, 0x0a);
			else USART_SendData(USART1, *Str);
	//�ȴ��������.
  	while (!(USART1->SR & USART_FLAG_TXE));
	//ָ��++ ָ����һ���ֽ�.
	Str++;
	}		 */
}


//------------------------------------------------------
void USART1_IRQHandler(void)
{

  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    char recvData;
    // clear receive interrupt flag
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    recvData = USART_ReceiveData(USART1) & 0x7F;
    USART_SendData(USART1, recvData);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
  }

  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
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


//------------------End of File----------------------------
