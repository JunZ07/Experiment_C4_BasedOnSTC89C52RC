#include <REGX52.H>
#include "Timer1.h"

unsigned char IRRepeatFlag, IRRecDataFlag;		//连续按住标志，获得按键值标志
unsigned char IRAddress, IRKeyValue;			//红外发出设备地址，按键值
unsigned char IRDataGroup[4], IRDataBitCount;	//红外接收中通过NEC收到的数据组，收到的数据位位数
unsigned int IRInterval;						//两次高电平的间隔时间，用于判断是什么指令
unsigned char IRNowState;						//状态机标志，0-未开始，1-接收到头部，2-接受数据

/**
  * @brief  外部中断1打开，设高优先级
  * @param  无
  * @retval 无
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
  * @brief  IR红外接收模块初始化，初始化定时器1，初始化外部中断
  * @param  无
  * @retval 无
  */
void IRInit()
{
	INT0Init();
	Timer1Init();
}

/**
  * @brief  获取是否收到数据标志并清空
  * @param  无
  * @retval 获得了数据返回1，否则0
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
  * @brief  获取是否接收到连按标志并清空
  * @param  无
  * @retval 收到repeat信号则返回1，否则0
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
  * @brief  获取发出设备地址
  * @param  无
  * @retval 返回发出设备地址
  */
unsigned char IRGetIRAddress()
{
	return IRAddress;
}

/**
  * @brief  获取发出设备按键值
  * @param  无
  * @retval 返回发出设备按键值
  */
unsigned char IRGetIRKeyValue()
{
	return IRKeyValue;
}

/**
  * @brief  外部中断函数，中断号0，扫描红外接收头收到的信号，低电平有效
  * @param  无
  * @retval 无
  */
void INT0Routine() interrupt 0
{
	//状态0，没有收到开始或者重复的信号，第一次低电平进入中断，定时器开始计时，并进入状态1
	if (IRNowState == 0)
	{
		Timer1SetCount(0);
		Timer1Run(1);
		IRNowState = 1;
	}
	
	//状态1，已经开始接受信号，判断信号是开始还是重复
	else if (IRNowState == 1)
	{
		IRInterval = Timer1GetCount();	//获取和上次低电平间隙时间，即信号的时长
		Timer1SetCount(0);				//获取后定时器重新开始计时
		if (IRInterval>12442-500 && IRInterval<12442+500) {			//开始信号，进入状态2
			IRNowState = 2;
		}
		else if (IRInterval>10368-500 && IRInterval<10368+500) {	//repeat重复信号，回到状态1，repeat期间没有低电平
			IRNowState = 0;
			IRRepeatFlag = 1;
			Timer1Run(0);
			Timer1SetCount(0);
		}
		else {														//错误的信号长度，继续等待
			IRNowState = 1;
		}
	}
	
	//状态2，收到了开始信号，开始接收4字节数据，接收完整回到状态1
	else if (IRNowState == 2)
	{
		IRInterval = Timer1GetCount();
		Timer1SetCount(0);
		if (IRInterval>1032-500 && IRInterval<1032+500) {					//信号0，数据数组写入0
			IRDataGroup[IRDataBitCount/8] &= ~(0x01 << IRDataBitCount%8);
			IRDataBitCount++;
		}
		else if (IRInterval>2074-500 && IRInterval<2074+500) {				//信号1，数据数组写入0
			IRDataGroup[IRDataBitCount/8] |= (0x01 << IRDataBitCount%8);
			IRDataBitCount++;
		}
		else {																//错误信号，接收数据出错，回到状态1重新等待
			IRDataBitCount = 0;
			IRNowState = 1;
		}
			
		//4字节数据全部读取完整，回到状态0
		if (IRDataBitCount >= 32) {
			//4字节数据前两字节为地址和地址的反，后两字节为码值和反，要验证一下接收无差错
			if (IRDataGroup[0]==~IRDataGroup[1] && IRDataGroup[2]==~IRDataGroup[3]) {
				IRRecDataFlag = 1;				//更新标志位说明收到了数据一次，并更新数据值
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