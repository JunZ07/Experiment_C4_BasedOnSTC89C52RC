/**************************************************
@introduction
��ʱ��T1�����ں���ģ��ͳ�Ƶ͵�ƽ��϶ʱ��
��Ϊ16λģʽ���������ж�
last modify time:20220822
**************************************************/

#ifndef __TIMER1_H__
#define __TIMER1_H__

void Timer1Init();							//��ʼ��
void Timer1SetCount(unsigned int count);	//���ü���ֵ
unsigned int Timer1GetCount();				//��ȡ����ֵ
void Timer1Run(unsigned char runFlag) ;		//�����Ƿ����


#endif