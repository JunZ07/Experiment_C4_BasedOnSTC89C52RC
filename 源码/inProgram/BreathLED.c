#include <REGX52.H>

#define LED_IO P2

void LEDOpen(void)
{
	LED_IO &= 0xF0;	//ֻ����ǰ4����
}

void LEDClose(void)
{
	LED_IO |= 0x0F;
}