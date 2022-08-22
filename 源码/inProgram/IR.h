/**************************************************
@introduction
����ң����ɨ����հ���ֵ��ģ��
�õ�NECЭ�飬ʹ��T1��ʱ����ʵ��NECЭ�����ź�ʱ�䳤�ȵ�ͳ��
T1��ʱ��������ʹ��Ƭ��ʱ�����壬����������
�ⲿ�ж�ɨ������ź�
last modify time:20220822
**************************************************/

#ifndef __IR_H__
#define __IR_H__

//����ң����������ֵ�ĺ궨��
#define IRKEY_POWER 0x45
#define IRKEY_MODE 0x46
#define IRKEY_SPEAKER 0x47
#define IRKEY_STOP 0x44
#define IRKEY_LAST 0x40
#define IRKEY_NEXT 0x43
#define IRKEY_EQ 0x07
#define IRKEY_VOLDOWN 0x15
#define IRKEY_VOLUP 0x09
#define IRKEY_0 0x16
#define IRKEY_RPT 0x19
#define IRKEY_USD 0x0D
#define IRKEY_1 0x0C
#define IRKEY_2 0x18
#define IRKEY_3 0x5E
#define IRKEY_4 0x08
#define IRKEY_5 0x1C
#define IRKEY_6 0x5A
#define IRKEY_7 0x42
#define IRKEY_8 0x52
#define IRKEY_9 0x4A

void IRInit();							//�������ͷ��ʼ��
unsigned char IRIsGetDataAndReset();	//��ð���ֵ����ջ�ð���ֵ��־λ
unsigned char IRIsRepeatDataAndReset();	//�������������־�����
unsigned char IRGetIRAddress();			//��ȡ���ⷢ���豸��ַ
unsigned char IRGetIRKeyValue();		//��ȡ���ⷢ���豸Ҳ����ң�صİ���ֵ


#endif