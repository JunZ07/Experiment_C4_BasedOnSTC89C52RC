//程序入口 调用初始化接口一次 循环依次调用红外识别-按键识别-计时器-显示器
//Control为提供控制接口的控制器

#include "Control.h"

void main()
{
	systemInit();				//初始化系统，包括定时器，红外中断，显示屏等初始化
	
	while(1) {
		scanIRAndExecute();	 	//识别红外按键并执行逻辑
		scanKeyAndExecute();	//识别矩阵键盘按键并执行逻辑
		countDown();			//实时修改倒计时计数
		show();					//根据修改的逻辑显示信息
	}
	
}