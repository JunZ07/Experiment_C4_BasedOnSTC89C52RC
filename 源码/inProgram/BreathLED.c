#include <REGX52.H>

#define LED_IO P2

void LEDOpen(void)
{
	LED_IO &= 0xF0;	//只控制前4个灯
}

void LEDClose(void)
{
	LED_IO |= 0x0F;
}