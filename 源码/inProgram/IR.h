/**************************************************
@introduction
红外遥控器扫描接收按键值的模块
用到NEC协议，使用T1定时器来实现NEC协议中信号时间长度的统计
T1定时器在这里使用片内时钟脉冲，用作计数器
外部中断扫描红外信号
last modify time:20220822
**************************************************/

#ifndef __IR_H__
#define __IR_H__

//红外遥控器按键码值的宏定义
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

void IRInit();							//红外接收头初始化
unsigned char IRIsGetDataAndReset();	//获得按键值并清空获得按键值标志位
unsigned char IRIsRepeatDataAndReset();	//获得连续按键标志并清空
unsigned char IRGetIRAddress();			//获取红外发送设备地址
unsigned char IRGetIRKeyValue();		//获取红外发送设备也就是遥控的按键值


#endif