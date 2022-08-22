#include <REGX52.H>

sbit RTC_CE = P3^5;		//使能端
sbit RTC_IO = P3^4;		//数据端
sbit RTC_SCLK = P3^6;	//时钟脉冲端

/**
  * @brief  向RTC实时时钟写入1字节数据
  * @param  command 1字节命令字
  * @param  dataByte 1字节数据
  * @retval 
  */
void RTCWriteByte(unsigned char command, dataByte)
{
	unsigned char i;
	
	RTC_CE = 1;
	RTC_IO = 0;
	RTC_SCLK = 0;
	
	for (i=0; i<8; i++) {
		RTC_IO = command & (0x01<<i);
		RTC_SCLK = 1;
		RTC_SCLK = 0;
	}
	for (i=0; i<8; i++) {
		RTC_IO = dataByte & (0x01<<i);
		RTC_SCLK = 1;
		RTC_SCLK = 0;
	}
	
	RTC_CE = 0;
}

/**
  * @brief  向RTC实时时钟读出1字节数据
  * @param  command 1字节命令字
  * @retval 1字节数据
  */
unsigned char RTCReadByte(unsigned char command)
{
	unsigned char dataByte = 0x00, i;
	
	RTC_CE = 1;
	RTC_IO = 0;
	RTC_SCLK = 0;
	
	for (i=0; i<8; i++) {
		RTC_IO = command & (0x01<<i);
		RTC_SCLK = 0;
		RTC_SCLK = 1;
	}for (i=0; i<8; i++) {
		RTC_SCLK = 1;
		RTC_SCLK = 0;
		if (RTC_IO) dataByte |= (0x01<<i);
	}
	
	RTC_CE = 0;
	return dataByte;
}

/**
  * @brief  清空RTC秒数
  * @param  无
  * @retval 无
  */
void RTCSecondClear(void)
{
	RTCWriteByte(0x80, 0x00);
}

/**
  * @brief  读取RTC秒数
  * @param  无
  * @retval 秒数
  */
unsigned char RTCReadSecond(void)
{
	unsigned char second = RTCReadByte(0x81);
	second = second/16*10 + second%16;
	return second;
}

/**
  * @brief  修改RTC秒数
  * @param  second 秒数
  * @retval 无
  */
void RTCSetSecond(unsigned char second)
{
	second = second/10*16 + second%10;
	RTCWriteByte(0x80, second);
}