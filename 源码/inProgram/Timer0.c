#include <REGX52.H>
#include "MatrixKey.h"
#include "BreathLED.h"

#define T0INIT_TL 0x66			//设置定时器初始值。1ms记1次
#define T0INIT_TH 0xFC
#define MATRIXKEY_COUNTFLAG 10	//调用扫描矩阵键盘的计时阈值，10是10ms

unsigned int LEDCycleCount[5] = {0, 3000, 1500, 500, 200};	//存4档LED一个亮灭周期，0不用
unsigned char LEDCycleSpeedMode;							//频率模式标号

/**
  * @brief  T0初始化，16位，打开中断，初始即开始计时
  * @param  无
  * @retval 无
  */
void Timer0Init()
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TF0 = 0;
	TL0 = T0INIT_TL;
	TH0 = T0INIT_TH;
	TR0 = 1;
	
	EA = 1;
	ET0 = 1;
	PT0 = 0;
}

/**
  * @brief  T0中断函数，包含扫描矩阵键盘和控制LED灯亮灭
  * @param  无
  * @retval 无
  */
void T0Routine(void) interrupt 1
{
	static unsigned char MatrixKeyCount = 0;		//矩阵键盘扫描计数
	static unsigned int LEDCount = 0;				//LED计数
	MatrixKeyCount++;
	TL0 = T0INIT_TL;	//重置计数值
	TH0 = T0INIT_TH;
	
	if (MatrixKeyCount >= MATRIXKEY_COUNTFLAG) {	//矩阵键盘计数到了阈值，扫描一次键盘
		MatrixKeyCount = 0;
		MatrixKeyLoop();
	}
	
	if (LEDCycleSpeedMode == 0)						//0档不亮
		LEDClose();
	else if (LEDCycleSpeedMode == 5)				//5档常亮
		LEDOpen();
	else {											//其他档位，按各档位亮灭周期，一半时间亮，一半时间灭，实现闪烁
		LEDCount++;
		if (LEDCount < LEDCycleCount[LEDCycleSpeedMode]/2)
			LEDOpen();
		else
			LEDClose();
		LEDCount%=LEDCycleCount[LEDCycleSpeedMode];
	}	
}

/**
  * @brief  设置LED灯闪烁频率
  * @param  mode 模式标号
  * @retval 无
  */
void setLEDCycleSpeedMode(unsigned char mode)
{
	LEDCycleSpeedMode = mode;
}