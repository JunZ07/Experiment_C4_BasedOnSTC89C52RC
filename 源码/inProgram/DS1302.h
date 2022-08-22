/**************************************************
@introduction
RTC实时时钟DS1302的驱动模块
主要用于获取实时准确的倒计时
last modify time: 20220822am
**************************************************/

#ifndef __DS1302_H__
#define __DS1302_H__

//void RTCWriteByte(unsigned char command, dataByte);
//unsigned char RTCReadByte(unsigned char command);
void RTCSecondClear(void);					//清空RTC秒数
unsigned char RTCReadSecond(void);			//设置RTC秒数
void RTCSetSecond(unsigned char second);	//读取RTC秒数


#endif