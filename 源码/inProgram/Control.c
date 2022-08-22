#include <REGX52.H>
#include <INTRINS.H>

#include "DS1302.h"		//RTC模块
#include "LCD1602.h"	//液晶显示屏模块
#include "IR.h"			//红外模块
#include "MatrixKey.h"	//矩阵键盘模块
#include "Timer0.h"		//定时器0
#include "Control.h"	//控制器声明
#include "Delay.h"		//时延函数模块
#include "BreathLED.h"	//LED灯模块

unsigned long inPassWord;	//输入的密码
unsigned char inPWCount;	//已输入密码位数
/*
4个标志位用于显示器的逻辑控制
isC4RunningFlag C4是否在运行中的标志
isPWCorrectFlag 确认键按下后密码是否正确的标志
isPWErrorFlag	确认键按下后密码是否错误的标志
isC4StopFlag	C4是否在运行中被暂停计时的标志
*/
unsigned char isC4RunningFlag, isPWCorrectFlag, isPWErrorFlag, isC4StopFlag;
unsigned char countTime;	//当前计时时间

/**
  * @brief  控制器也是整个系统的初始化
  * @param  无
  * @retval 无
  */
void systemInit(void)
{
	LCDInit();					//液晶显示屏初始化
	IRInit();					//红外模块初始化，打开红外中断，里面包括计时器1初始化
	Timer0Init();				//计时器0初始化，开始计时
	LCDPrintCover();			//液晶显示屏打印出来C4封面信息
	setLEDCycleSpeedMode(0);	//设置LED灯的频率模式 0为初始即不亮
}

/**
  * @brief  按键扫描和逻辑执行，修改标志位
  * @param  无
  * @retval 无
  */
void scanKeyAndExecute(void)
{
	unsigned char KeyValue = MatrixKey();	//读扫描到的按键值
	if (KeyValue) {
		switch(KeyValue) {
			case 1:	//数字1
			case 2:	//数字2	
			case 3:	//数字3
			case 5:	//数字4	
			case 6:	//数字5	
			case 7:	//数字6	
			case 9:	//数字7
			case 10://数字8	
			case 11://数字9	
			case 14://数字0
				if (inPWCount<PASSWORDLEN) {	//没有输完整密码，则把输入的下一位读到密码里面
					if (KeyValue>3 && KeyValue<9)KeyValue--;
					if (KeyValue>8 && KeyValue<14)KeyValue -= 2;
					if (KeyValue==14)KeyValue=0;
					inPassWord = inPassWord*10 + KeyValue;		
					inPWCount++;
				}
				break;
			case 4:	//确认键
				if (inPWCount>=PASSWORDLEN) {	//密码已经输入完整
					if (inPassWord == PASSWORD) {//密码正确置标志位为1
						isPWCorrectFlag = 1;
					}
					else {
						isPWErrorFlag = 1;		//密码错误置标志位为1
					}
					inPassWord = 0;				//清空密码
					inPWCount = 0;
				}
				break;
			case 8: //清空键
				inPassWord = 0;
				inPWCount = 0;
				LCDShowString(2, 1, "XXXXXXX");	//恢复没有密码的显示
				break;
			default:
				break;
		}
	}
}

/**
  * @brief  红外扫描遥控器按键，逻辑执行，修改标志位
  * @param  无
  * @retval 无
  */
void scanIRAndExecute(void)
{
	unsigned char KeyValue;
	if (IRIsGetDataAndReset()) {		//红外扫描到按键按下
		KeyValue = IRGetIRKeyValue();	//获取遥控器按键值
		if (KeyValue == IRKEY_POWER) {	//电源键，回到C4初始化状态
			isC4RunningFlag = 0;
			isPWCorrectFlag = 0;
			isPWErrorFlag = 0;
			isC4StopFlag = 0;
			inPassWord = 0;
			inPWCount = 0;
			LCDPrintCover();			//显示封面
		}
		if (KeyValue == IRKEY_STOP) {	//暂停键
			if (isC4RunningFlag) {		//C4正在运行置暂停标志为1
				isC4StopFlag = ~isC4StopFlag;
			}
		}
	}
}

/**
  * @brief  计时器倒计时并修改LED灯频率
  * @param  无
  * @retval 无
  */
void countDown(void)
{
	if (isC4RunningFlag) {					//C4运行中
		if (isC4StopFlag) {					//被遥控器暂停，则刷新RTC时间为暂停之前的时间
			RTCSetSecond(countTime);		
		}
		else {
			countTime = RTCReadSecond();	//没有暂停，则实时更新倒计时时间，从RTC读取
		}
	}
	else {									//没有运行，倒计时记0，RTC秒数清空
		countTime = 0;
		RTCSecondClear();
	}
	
	//根据倒计时剩余时间调节LED灯亮灭的频率
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
  * @brief  显示屏根据标志位显示
  * @param  无
  * @retval 无
  */
void show()
{
	//C4正在运行
	if (isC4RunningFlag) {
		if (countTime >= COUNTDOWN) {		//倒计时结束，进入卡死状态
			LCDPrintLine();
			while(1);
		}
		if (isPWCorrectFlag) {				//密码输入正确，停止C4
			LCDPrintLine();
			Delay(2000);
			LCDPrintCover();
			isC4RunningFlag = ~isC4RunningFlag;
			isPWCorrectFlag = 0;
			return;
		}
		if (isPWErrorFlag) {				//密码输入错误，提示错误
			LCDPrintError();
			isPWErrorFlag = 0;
		}
		LCDPrintCountDown(COUNTDOWN-countTime);		//刷新一次倒计时
		if (inPWCount!=0)							//当前正在输入密码，显示已输入部分
			LCDShowInPW(2, 1, inPassWord, inPWCount);
	}
	
	//C4不运行
	else {
		if (isPWCorrectFlag) {				//密码输入正确，运行C4
			LCDPrintPlanted();
			Delay(2000);
			LCDPrintCountDown(COUNTDOWN-countTime);
			LCDShowClean(2);
			isC4RunningFlag = ~isC4RunningFlag;
			isPWCorrectFlag = 0;
		}
		if (isPWErrorFlag) {				//密码输入错误，提示错误
			LCDPrintError();
			Delay(1000);
			isPWErrorFlag = 0;
		}
		if (inPWCount!=0)					//当前正在输入密码，显示已输入部分
			LCDShowInPW(2, 1, inPassWord, inPWCount);
		else								//没有输入密码封面覆盖
			LCDPrintCover();
	}
}
