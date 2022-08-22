/**************************************************
@introduction
定时器0，开启中断，实现不断扫描矩阵键盘按下的按键以及控制LED灯闪烁的频率
16位模式，每1ms记一次
last modify time:20220822
**************************************************/

#ifndef __TIMER0_H__
#define __TIMER0_H__

void Timer0Init();								//初始化
void setLEDCycleSpeedMode(unsigned char mode);	//提供给控制器设置LED灯闪烁频率的接口


#endif