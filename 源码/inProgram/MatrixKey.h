/**************************************************
@introduction
矩阵键盘驱动，扫描按键值接口提供给定时器定时扫描
last modify time:20220822
**************************************************/

#ifndef __MATRIXKEY_H__
#define __MATRIXKEY_H__

void MatrixKeyLoop(void);		//扫描按键值
unsigned char MatrixKey(void);	//获取按键值

#endif
