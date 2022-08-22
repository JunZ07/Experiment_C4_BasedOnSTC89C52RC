/**************************************************
@introduction
液晶显示屏驱动和显示接口模块，用于各种提示信息的显示
液晶显示屏用的是LCD1602
last modify time:20220822
**************************************************/

#ifndef __LCD1602_H__
#define __LCD1602_H__

void LCDInit(void);		//显示屏初始化，必须先初始化

//基础的数据显示，字符，字符串，无符号int，有符号int，16进制
void LCDShowChar(unsigned char line, unsigned char column, unsigned char char0);
void LCDShowString(unsigned char Line,unsigned char Column,unsigned char *String);
void LCDShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCDShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length);
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);

//提供给Control的常用的显示屏显示提示信息的接口
void LCDShowClean(unsigned char line);			//清空
void LCDPrintCover(void);						//输出封面
void LCDPrintPlanted(void);						//输出下包信息
void LCDPrintCountDown(unsigned char count);	//输出倒计时
void LCDPrintLine(void);						//在某行打印直线
void LCDPrintError(void);						//输出错误信息
//显示已输入的部分密码
void LCDShowInPW(unsigned char Line,unsigned char Column,unsigned long Number,unsigned char PWCount);

#endif