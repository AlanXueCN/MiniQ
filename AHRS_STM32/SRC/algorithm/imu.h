#ifndef __IMU_H
#define __IMU_H

#include <stdint.h>


//Mini IMU AHRS �����API
void IMU_init(void); //��ʼ��
void IMU_getYawPitchRoll(float * ypr,float *data); //������̬
uint32_t micros(void);	//��ȡϵͳ�ϵ���ʱ��  ��λ us 

#endif
