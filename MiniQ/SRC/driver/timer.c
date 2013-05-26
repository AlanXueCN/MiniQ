#include "timer.h"

void Initial_TimerTick(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE); 
	/* TIM3 configuration*/ 
  /* Time Base configuration �������� ���ö�ʱ����ʱ����Ԫ*/
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 0xffff; //�Զ���װֵ         
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
  
  TIM_PrescalerConfig(TIM3, 0, TIM_PSCReloadMode_Update);
  /* Disable the TIM3 Update event */
  TIM_UpdateDisableConfig(TIM3, ENABLE);
  /* ----------------------TIM3 Configuration as slave for the TIM4 ----------*/
  /* Select the TIM3 Input Trigger: TIM4 TRGO used as Input Trigger for TIM3*/
  TIM_SelectInputTrigger(TIM3, TIM_TS_ITR3);
  /* Use the External Clock as TIM3 Slave Mode */
  TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_External1);
  /* Enable the TIM3 Master Slave Mode */
  TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);
  TIM_ARRPreloadConfig(TIM3, ENABLE);	
	/* ��ʱ������:
	1.���ö�ʱ��������ֵ 50000
	2.����ʱ�ӷ�Ƶϵ����TIM_CKD_DIV1
	3. ����Ԥ��Ƶ��  1Mhz/50000= 1hz 
	4.��ʱ������ģʽ  ���ϼ���ģʽ
	*/		 
  	TIM_TimeBaseStructure.TIM_Period = 0xffff;     
  	TIM_TimeBaseStructure.TIM_Prescaler = 72;	 //1M ��ʱ��  
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	//Ӧ�����õ�TIM4 
  	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	// ʹ��TIM4���ؼĴ���ARR
  	TIM_ARRPreloadConfig(TIM4, ENABLE);	

	TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
	TIM_UpdateRequestConfig(TIM4, TIM_UpdateSource_Regular);
	/* ----------------------TIM4 Configuration as Master for the TIM3 -----------*/
  	/* Use the TIM4 Update event  as TIM4 Trigger Output(TRGO) */
  	TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);
  	/* Enable the TIM4 Master Slave Mode */
  	TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);

  	//������ʱ��
	TIM_Cmd(TIM4, ENABLE); 
  	TIM_Cmd(TIM3, ENABLE);  
}

uint32_t micros(void)
{
 	uint32_t temp=0 ;
 	temp = TIM3->CNT; //����16λʱ��
 	temp = temp<<16;
 	temp += TIM4->CNT; //����16λʱ��
 	return temp;
}
