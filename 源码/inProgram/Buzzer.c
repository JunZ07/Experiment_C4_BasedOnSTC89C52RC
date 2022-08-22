#include <REGX52.H>

sbit BZ_BEEP = P2^5;

void BuzzerShake(void)
{
	BZ_BEEP = !BZ_BEEP;
}