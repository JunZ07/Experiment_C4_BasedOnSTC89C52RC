#include <REGX52.H>
#include <INTRINS.H>

#include "DS1302.h"		//RTCģ��
#include "LCD1602.h"	//Һ����ʾ��ģ��
#include "IR.h"			//����ģ��
#include "MatrixKey.h"	//�������ģ��
#include "Timer0.h"		//��ʱ��0
#include "Control.h"	//����������
#include "Delay.h"		//ʱ�Ӻ���ģ��
#include "BreathLED.h"	//LED��ģ��

unsigned long inPassWord;	//���������
unsigned char inPWCount;	//����������λ��
/*
4����־λ������ʾ�����߼�����
isC4RunningFlag C4�Ƿ��������еı�־
isPWCorrectFlag ȷ�ϼ����º������Ƿ���ȷ�ı�־
isPWErrorFlag	ȷ�ϼ����º������Ƿ����ı�־
isC4StopFlag	C4�Ƿ��������б���ͣ��ʱ�ı�־
*/
unsigned char isC4RunningFlag, isPWCorrectFlag, isPWErrorFlag, isC4StopFlag;
unsigned char countTime;	//��ǰ��ʱʱ��

/**
  * @brief  ������Ҳ������ϵͳ�ĳ�ʼ��
  * @param  ��
  * @retval ��
  */
void systemInit(void)
{
	LCDInit();					//Һ����ʾ����ʼ��
	IRInit();					//����ģ���ʼ�����򿪺����жϣ����������ʱ��1��ʼ��
	Timer0Init();				//��ʱ��0��ʼ������ʼ��ʱ
	LCDPrintCover();			//Һ����ʾ����ӡ����C4������Ϣ
	setLEDCycleSpeedMode(0);	//����LED�Ƶ�Ƶ��ģʽ 0Ϊ��ʼ������
}

/**
  * @brief  ����ɨ����߼�ִ�У��޸ı�־λ
  * @param  ��
  * @retval ��
  */
void scanKeyAndExecute(void)
{
	unsigned char KeyValue = MatrixKey();	//��ɨ�赽�İ���ֵ
	if (KeyValue) {
		switch(KeyValue) {
			case 1:	//����1
			case 2:	//����2	
			case 3:	//����3
			case 5:	//����4	
			case 6:	//����5	
			case 7:	//����6	
			case 9:	//����7
			case 10://����8	
			case 11://����9	
			case 14://����0
				if (inPWCount<PASSWORDLEN) {	//û�����������룬����������һλ������������
					if (KeyValue>3 && KeyValue<9)KeyValue--;
					if (KeyValue>8 && KeyValue<14)KeyValue -= 2;
					if (KeyValue==14)KeyValue=0;
					inPassWord = inPassWord*10 + KeyValue;		
					inPWCount++;
				}
				break;
			case 4:	//ȷ�ϼ�
				if (inPWCount>=PASSWORDLEN) {	//�����Ѿ���������
					if (inPassWord == PASSWORD) {//������ȷ�ñ�־λΪ1
						isPWCorrectFlag = 1;
					}
					else {
						isPWErrorFlag = 1;		//��������ñ�־λΪ1
					}
					inPassWord = 0;				//�������
					inPWCount = 0;
				}
				break;
			case 8: //��ռ�
				inPassWord = 0;
				inPWCount = 0;
				LCDShowString(2, 1, "XXXXXXX");	//�ָ�û���������ʾ
				break;
			default:
				break;
		}
	}
}

/**
  * @brief  ����ɨ��ң�����������߼�ִ�У��޸ı�־λ
  * @param  ��
  * @retval ��
  */
void scanIRAndExecute(void)
{
	unsigned char KeyValue;
	if (IRIsGetDataAndReset()) {		//����ɨ�赽��������
		KeyValue = IRGetIRKeyValue();	//��ȡң��������ֵ
		if (KeyValue == IRKEY_POWER) {	//��Դ�����ص�C4��ʼ��״̬
			isC4RunningFlag = 0;
			isPWCorrectFlag = 0;
			isPWErrorFlag = 0;
			isC4StopFlag = 0;
			inPassWord = 0;
			inPWCount = 0;
			LCDPrintCover();			//��ʾ����
		}
		if (KeyValue == IRKEY_STOP) {	//��ͣ��
			if (isC4RunningFlag) {		//C4������������ͣ��־Ϊ1
				isC4StopFlag = ~isC4StopFlag;
			}
		}
	}
}

/**
  * @brief  ��ʱ������ʱ���޸�LED��Ƶ��
  * @param  ��
  * @retval ��
  */
void countDown(void)
{
	if (isC4RunningFlag) {					//C4������
		if (isC4StopFlag) {					//��ң������ͣ����ˢ��RTCʱ��Ϊ��֮ͣǰ��ʱ��
			RTCSetSecond(countTime);		
		}
		else {
			countTime = RTCReadSecond();	//û����ͣ����ʵʱ���µ���ʱʱ�䣬��RTC��ȡ
		}
	}
	else {									//û�����У�����ʱ��0��RTC�������
		countTime = 0;
		RTCSecondClear();
	}
	
	//���ݵ���ʱʣ��ʱ�����LED�������Ƶ��
	if (COUNTDOWN-countTime>=30 || isC4StopFlag)
		setLEDCycleSpeedMode(0);
	if (COUNTDOWN-countTime>30 && COUNTDOWN-countTime<45)
		setLEDCycleSpeedMode(1);
	if (COUNTDOWN-countTime>20 && COUNTDOWN-countTime<=30)
		setLEDCycleSpeedMode(2);
	if (COUNTDOWN-countTime>10 && COUNTDOWN-countTime<=20)
		setLEDCycleSpeedMode(3);
	if (COUNTDOWN-countTime>3 && COUNTDOWN-countTime<=10)
		setLEDCycleSpeedMode(4);
	if (COUNTDOWN-countTime>0 && COUNTDOWN-countTime<=3)
		setLEDCycleSpeedMode(5);
}

/**
  * @brief  ��ʾ�����ݱ�־λ��ʾ
  * @param  ��
  * @retval ��
  */
void show()
{
	//C4��������
	if (isC4RunningFlag) {
		if (countTime >= COUNTDOWN) {		//����ʱ���������뿨��״̬
			LCDPrintLine();
			while(1);
		}
		if (isPWCorrectFlag) {				//����������ȷ��ֹͣC4
			LCDPrintLine();
			Delay(2000);
			LCDPrintCover();
			isC4RunningFlag = ~isC4RunningFlag;
			isPWCorrectFlag = 0;
			return;
		}
		if (isPWErrorFlag) {				//�������������ʾ����
			LCDPrintError();
			isPWErrorFlag = 0;
		}
		LCDPrintCountDown(COUNTDOWN-countTime);		//ˢ��һ�ε���ʱ
		if (inPWCount!=0)							//��ǰ�����������룬��ʾ�����벿��
			LCDShowInPW(2, 1, inPassWord, inPWCount);
	}
	
	//C4������
	else {
		if (isPWCorrectFlag) {				//����������ȷ������C4
			LCDPrintPlanted();
			Delay(2000);
			LCDPrintCountDown(COUNTDOWN-countTime);
			LCDShowClean(2);
			isC4RunningFlag = ~isC4RunningFlag;
			isPWCorrectFlag = 0;
		}
		if (isPWErrorFlag) {				//�������������ʾ����
			LCDPrintError();
			Delay(1000);
			isPWErrorFlag = 0;
		}
		if (inPWCount!=0)					//��ǰ�����������룬��ʾ�����벿��
			LCDShowInPW(2, 1, inPassWord, inPWCount);
		else								//û������������渲��
			LCDPrintCover();
	}
}
