#ifndef IOI2C_H_
#define IOI2C_H_

#include "stm32f10x.h"
#include <stdbool.h>
#define IMU_I2C                    I2C1
#define IMU_I2C_GPIO               GPIOB
#define IMU_I2C_CLK                RCC_APB1Periph_I2C1
#define IMU_I2C_GPIO_CLK           RCC_APB2Periph_GPIOB
#define IMU_I2C_SclPin             GPIO_Pin_6
#define IMU_I2C_SdaPin             GPIO_Pin_7
/***** �����궨�� *****/
#define SCL_H         (IMU_I2C_GPIO->BSRR = IMU_I2C_SclPin)
#define SCL_L         (IMU_I2C_GPIO->BRR  = IMU_I2C_SclPin)    
#define SDA_H         (IMU_I2C_GPIO->BSRR = IMU_I2C_SdaPin)
#define SDA_L         (IMU_I2C_GPIO->BRR  = IMU_I2C_SdaPin)
#define SCL_read      (IMU_I2C_GPIO->IDR  & IMU_I2C_SclPin)
#define SDA_read      (IMU_I2C_GPIO->IDR  & IMU_I2C_SdaPin)

#define I2C_DELAY_VAL   2*9

 void    I2C_Init(void);               //����I2C�ӿ�
 bool    Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
 uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address);
 bool    Fast_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
 bool    Mult_Read(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t * ptChar,uint8_t size);

//I2C������������
 bool I2C_Start(void);              //����æ�������򷵻�false
 void I2C_Stop(void);
 void I2C_Ack(void);
 void I2C_NoAck(void);
 bool I2C_WaitAck(void); 	          //����Ϊ:=1��ACK,=0��ACK
 void I2C_SendByte(u8 SendByte);    //���ݴӸ�λ����λ
 uint8_t I2C_RadeByte(void);  //���ݴӸ�λ����λ

#endif



