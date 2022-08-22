#include <REGX52.H>

/**
  * @brief  T1初始化，16位模式，初始计数值0，暂停计数
  * @param  无
  * @retval 无
  */
void Timer1Init()
{
	TMOD &= 0x0F;
	TMOD |= 0x10;
	TF1 = 0;
	TH1 = 0;
	TL1 = 0;
	TR1 = 0;	//不计数
}

/**
  * @brief  设置T1计数
  * @param  无
  * @retval 无
  */
void Timer1SetCount(unsigned int count)
{
	TH1 = count/256;
	TL1 = count%256;
}

/**
  * @brief  获得T1计数
  * @param  无
  * @retval 计数值
  */
unsigned int Timer1GetCount()
{
	return ((TH1<<8) | TL1);	//利用强制转换，高位前移8位
}

/**
  * @brief  控制计时器是否计数
  * @param  runFlag 1开始计数，0暂停
  * @retval 无
  */
void Timer1Run(unsigned char runFlag) 
{
	if (runFlag == 1)
		TR1 = 1;
	else
		TR1 = 0;
}

