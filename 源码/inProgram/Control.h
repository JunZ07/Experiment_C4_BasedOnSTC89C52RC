/**************************************************
@introduction
��������ϵͳ���еĿ�����ģ�飬���������ṩ���п����õĽӿ�
last modify time:20220822
**************************************************/

#ifndef __CONTROL_H__
#define __CONTROL_H__

//�ֱ�Ϊ����ʱ��ʱ����0~59�� �����루0~1e14�� �����볤��1~14��
#define COUNTDOWN  45
#define PASSWORD 7355608
#define PASSWORDLEN 7

void systemInit(void);				//ϵͳ��ʼ��
void scanKeyAndExecute(void);		//����ɨ���߼�ִ��
void scanIRAndExecute(void);		//����ɨ���߼�ִ��
void countDown(void);				//����ʱ���º�LEDƵ�ʵ���
void show(void);					//��ʾ��������ʾ

#endif