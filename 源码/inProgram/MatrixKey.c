#include <REGX52.H>

//矩阵键盘的IO口 C是列 L是行
sbit MKEY_C1 = P1^3;
sbit MKEY_C2 = P1^2;
sbit MKEY_C3 = P1^1;
sbit MKEY_C4 = P1^0;
sbit MKEY_L1 = P1^7;
sbit MKEY_L2 = P1^6;
sbit MKEY_L3 = P1^5;
sbit MKEY_L4 = P1^4;
#define MKEY_IO P1	//方便全部置0

unsigned char MatrixKeyNum;	//当前扫描到的按键值

/**
  * @brief  扫描矩阵键盘按键的IO口电平状态来得知哪个按键被按下
  * @param  无
  * @retval 被按下的按键值，最后一个有效，单个有效
  */
unsigned char MatrixKeyGetState()
{
	unsigned char KeyNumber=0;
	
	MKEY_IO = 0xFF;
	MKEY_C1 = 0;
	if(MKEY_L1 == 0){KeyNumber=1;}
	if(MKEY_L2 == 0){KeyNumber=5;}
	if(MKEY_L3 == 0){KeyNumber=9;}
	if(MKEY_L4 == 0){KeyNumber=13;}
	
	MKEY_IO = 0xFF;
	MKEY_C2 = 0;
	if(MKEY_L1 == 0){KeyNumber=2;}
	if(MKEY_L2 == 0){KeyNumber=6;}
	if(MKEY_L3 == 0){KeyNumber=10;}
	if(MKEY_L4 == 0){KeyNumber=14;}
	
	MKEY_IO = 0xFF;
	MKEY_C3 = 0;
	if(MKEY_L1 == 0){KeyNumber=3;}
	if(MKEY_L2 == 0){KeyNumber=7;}
	if(MKEY_L3 == 0){KeyNumber=11;}
	if(MKEY_L4 == 0){KeyNumber=15;}
	
	MKEY_IO = 0xFF;
	MKEY_C4 = 0;
	if(MKEY_L1 == 0){KeyNumber=4;}
	if(MKEY_L2 == 0){KeyNumber=8;}
	if(MKEY_L3 == 0){KeyNumber=12;}
	if(MKEY_L4 == 0){KeyNumber=16;}
	
	return KeyNumber;
}

/**
  * @brief  提供给定时器定时扫描的按键扫描函数，按下并松开时有效
  * @param  无
  * @retval 无
  */
void MatrixKeyLoop(void)
{
	static unsigned char lastState, nowState;	//上一次电平状态和这一次
	lastState = nowState;
	nowState = MatrixKeyGetState();
	if (lastState!=0 && nowState==0)			//上一次电平为低，然后这次为高，则说明被按下并跳起了
		MatrixKeyNum = lastState;
}

/**
  * @brief  获取矩阵键盘当前扫描到的按键值
  * @param  无
  * @retval 按键值
  */
unsigned char MatrixKey(void)
{
	unsigned char tempKeyNum = MatrixKeyNum;
	MatrixKeyNum = 0;
	return tempKeyNum;
}