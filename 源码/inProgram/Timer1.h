/**************************************************
@introduction
定时器T1，用于红外模块统计低电平间隙时长
设为16位模式，不开启中断
last modify time:20220822
**************************************************/

#ifndef __TIMER1_H__
#define __TIMER1_H__

void Timer1Init();							//初始化
void Timer1SetCount(unsigned int count);	//设置计数值
unsigned int Timer1GetCount();				//获取计数值
void Timer1Run(unsigned char runFlag) ;		//控制是否计数


#endif