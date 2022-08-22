#include <REGX52.H>

/**
  * @brief  T1��ʼ����16λģʽ����ʼ����ֵ0����ͣ����
  * @param  ��
  * @retval ��
  */
void Timer1Init()
{
	TMOD &= 0x0F;
	TMOD |= 0x10;
	TF1 = 0;
	TH1 = 0;
	TL1 = 0;
	TR1 = 0;	//������
}

/**
  * @brief  ����T1����
  * @param  ��
  * @retval ��
  */
void Timer1SetCount(unsigned int count)
{
	TH1 = count/256;
	TL1 = count%256;
}

/**
  * @brief  ���T1����
  * @param  ��
  * @retval ����ֵ
  */
unsigned int Timer1GetCount()
{
	return ((TH1<<8) | TL1);	//����ǿ��ת������λǰ��8λ
}

/**
  * @brief  ���Ƽ�ʱ���Ƿ����
  * @param  runFlag 1��ʼ������0��ͣ
  * @retval ��
  */
void Timer1Run(unsigned char runFlag) 
{
	if (runFlag == 1)
		TR1 = 1;
	else
		TR1 = 0;
}

