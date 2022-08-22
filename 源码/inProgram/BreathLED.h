/**************************************************
@introduction
原本是想做呼吸灯，所以叫BreathLED懒得改了
由于开发板上后4个灯引脚冲突所以只控制前4个灯亮灭
频闪在定时器0模块里实现 Timer0.c
last modify time:20220822
**************************************************/

#ifndef __BREATHLED_H__
#define __BREATHLED_H__

void LEDOpen(void);		//4个灯亮
void LEDClose(void);	//4个灯灭

#endif