/**************************************************
@introduction
控制整个系统运行的控制器模块，向程序入口提供所有控制用的接口
last modify time:20220822
**************************************************/

#ifndef __CONTROL_H__
#define __CONTROL_H__

//分别为倒计时总时长（0~59） ，密码（0~1e14） ，密码长（1~14）
#define COUNTDOWN  45
#define PASSWORD 7355608
#define PASSWORDLEN 7

void systemInit(void);				//系统初始化
void scanKeyAndExecute(void);		//按键扫描逻辑执行
void scanIRAndExecute(void);		//红外扫描逻辑执行
void countDown(void);				//倒计时更新和LED频率调节
void show(void);					//显示屏更新显示

#endif