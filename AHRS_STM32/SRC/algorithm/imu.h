#ifndef __IMU_H
#define __IMU_H

#include <stdint.h>


//Mini IMU AHRS �����API
void IMU_init(void); //��ʼ��
void IMU_getYawPitchRoll(int16_t * angles,int16_t *data); //������̬
uint32_t micros(void);	//��ȡϵͳ�ϵ���ʱ��  ��λ us 

#endif
