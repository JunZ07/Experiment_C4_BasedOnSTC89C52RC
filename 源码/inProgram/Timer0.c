#include <REGX52.H>
#include "MatrixKey.h"
#include "BreathLED.h"

#define T0INIT_TL 0x66			//���ö�ʱ����ʼֵ��1ms��1��
#define T0INIT_TH 0xFC
#define MATRIXKEY_COUNTFLAG 10	//����ɨ�������̵ļ�ʱ��ֵ��10��10ms

unsigned int LEDCycleCount[5] = {0, 3000, 1500, 500, 200};	//��4��LEDһ���������ڣ�0����
unsigned char LEDCycleSpeedMode;							//Ƶ��ģʽ���

/**
  * @brief  T0��ʼ����16λ�����жϣ���ʼ����ʼ��ʱ
  * @param  ��
  * @retval ��
  */
void Timer0Init()
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TF0 = 0;
	TL0 = T0INIT_TL;
	TH0 = T0INIT_TH;
	TR0 = 1;
	
	EA = 1;
	ET0 = 1;
	PT0 = 0;
}

/**
  * @brief  T0�жϺ���������ɨ�������̺Ϳ���LED������
  * @param  ��
  * @retval ��
  */
void T0Routine(void) interrupt 1
{
	static unsigned char MatrixKeyCount = 0;		//�������ɨ�����
	static unsigned int LEDCount = 0;				//LED����
	MatrixKeyCount++;
	TL0 = T0INIT_TL;	//���ü���ֵ
	TH0 = T0INIT_TH;
	
	if (MatrixKeyCount >= MATRIXKEY_COUNTFLAG) {	//������̼���������ֵ��ɨ��һ�μ���
		MatrixKeyCount = 0;
		MatrixKeyLoop();
	}
	
	if (LEDCycleSpeedMode == 0)						//0������
		LEDClose();
	else if (LEDCycleSpeedMode == 5)				//5������
		LEDOpen();
	else {											//������λ��������λ�������ڣ�һ��ʱ������һ��ʱ����ʵ����˸
		LEDCount++;
		if (LEDCount < LEDCycleCount[LEDCycleSpeedMode]/2)
			LEDOpen();
		else
			LEDClose();
		LEDCount%=LEDCycleCount[LEDCycleSpeedMode];
	}	
}

/**
  * @brief  ����LED����˸Ƶ��
  * @param  mode ģʽ���
  * @retval ��
  */
void setLEDCycleSpeedMode(unsigned char mode)
{
	LEDCycleSpeedMode = mode;
}