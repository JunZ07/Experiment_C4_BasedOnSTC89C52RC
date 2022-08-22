#include <REGX52.H>
#include "Timer1.h"

unsigned char IRRepeatFlag, IRRecDataFlag;		//������ס��־����ð���ֵ��־
unsigned char IRAddress, IRKeyValue;			//���ⷢ���豸��ַ������ֵ
unsigned char IRDataGroup[4], IRDataBitCount;	//���������ͨ��NEC�յ��������飬�յ�������λλ��
unsigned int IRInterval;						//���θߵ�ƽ�ļ��ʱ�䣬�����ж���ʲôָ��
unsigned char IRNowState;						//״̬����־��0-δ��ʼ��1-���յ�ͷ����2-��������

/**
  * @brief  �ⲿ�ж�1�򿪣�������ȼ�
  * @param  ��
  * @retval ��
  */
void INT0Init()
{
	EA = 1;
	EX0 = 1;
	IT0 = 1;
	PX0 = 1;
	IE0 = 0;
}

/**
  * @brief  IR�������ģ���ʼ������ʼ����ʱ��1����ʼ���ⲿ�ж�
  * @param  ��
  * @retval ��
  */
void IRInit()
{
	INT0Init();
	Timer1Init();
}

/**
  * @brief  ��ȡ�Ƿ��յ����ݱ�־�����
  * @param  ��
  * @retval ��������ݷ���1������0
  */
unsigned char IRIsGetDataAndReset()
{
	if (IRRecDataFlag) {
		IRRecDataFlag = 0;
		return 1;
	}
	else 
		return 0;
}

/**
  * @brief  ��ȡ�Ƿ���յ�������־�����
  * @param  ��
  * @retval �յ�repeat�ź��򷵻�1������0
  */
unsigned char IRIsRepeatDataAndReset()
{
	if (IRRepeatFlag) {
		IRRepeatFlag = 0;
		return 1;
	}
	else 
		return 0;
}

/**
  * @brief  ��ȡ�����豸��ַ
  * @param  ��
  * @retval ���ط����豸��ַ
  */
unsigned char IRGetIRAddress()
{
	return IRAddress;
}

/**
  * @brief  ��ȡ�����豸����ֵ
  * @param  ��
  * @retval ���ط����豸����ֵ
  */
unsigned char IRGetIRKeyValue()
{
	return IRKeyValue;
}

/**
  * @brief  �ⲿ�жϺ������жϺ�0��ɨ��������ͷ�յ����źţ��͵�ƽ��Ч
  * @param  ��
  * @retval ��
  */
void INT0Routine() interrupt 0
{
	//״̬0��û���յ���ʼ�����ظ����źţ���һ�ε͵�ƽ�����жϣ���ʱ����ʼ��ʱ��������״̬1
	if (IRNowState == 0)
	{
		Timer1SetCount(0);
		Timer1Run(1);
		IRNowState = 1;
	}
	
	//״̬1���Ѿ���ʼ�����źţ��ж��ź��ǿ�ʼ�����ظ�
	else if (IRNowState == 1)
	{
		IRInterval = Timer1GetCount();	//��ȡ���ϴε͵�ƽ��϶ʱ�䣬���źŵ�ʱ��
		Timer1SetCount(0);				//��ȡ��ʱ�����¿�ʼ��ʱ
		if (IRInterval>12442-500 && IRInterval<12442+500) {			//��ʼ�źţ�����״̬2
			IRNowState = 2;
		}
		else if (IRInterval>10368-500 && IRInterval<10368+500) {	//repeat�ظ��źţ��ص�״̬1��repeat�ڼ�û�е͵�ƽ
			IRNowState = 0;
			IRRepeatFlag = 1;
			Timer1Run(0);
			Timer1SetCount(0);
		}
		else {														//������źų��ȣ������ȴ�
			IRNowState = 1;
		}
	}
	
	//״̬2���յ��˿�ʼ�źţ���ʼ����4�ֽ����ݣ����������ص�״̬1
	else if (IRNowState == 2)
	{
		IRInterval = Timer1GetCount();
		Timer1SetCount(0);
		if (IRInterval>1032-500 && IRInterval<1032+500) {					//�ź�0����������д��0
			IRDataGroup[IRDataBitCount/8] &= ~(0x01 << IRDataBitCount%8);
			IRDataBitCount++;
		}
		else if (IRInterval>2074-500 && IRInterval<2074+500) {				//�ź�1����������д��0
			IRDataGroup[IRDataBitCount/8] |= (0x01 << IRDataBitCount%8);
			IRDataBitCount++;
		}
		else {																//�����źţ��������ݳ����ص�״̬1���µȴ�
			IRDataBitCount = 0;
			IRNowState = 1;
		}
			
		//4�ֽ�����ȫ����ȡ�������ص�״̬0
		if (IRDataBitCount >= 32) {
			//4�ֽ�����ǰ���ֽ�Ϊ��ַ�͵�ַ�ķ��������ֽ�Ϊ��ֵ�ͷ���Ҫ��֤һ�½����޲��
			if (IRDataGroup[0]==~IRDataGroup[1] && IRDataGroup[2]==~IRDataGroup[3]) {
				IRRecDataFlag = 1;				//���±�־λ˵���յ�������һ�Σ�����������ֵ
				IRAddress = IRDataGroup[0];	
				IRKeyValue = IRDataGroup[2];
			}
			IRDataBitCount = 0;
			IRNowState = 0;
			Timer1Run(0);
			Timer1SetCount(0);
		}
	}
}