#include <REGX52.H>

sbit RTC_CE = P3^5;		//ʹ�ܶ�
sbit RTC_IO = P3^4;		//���ݶ�
sbit RTC_SCLK = P3^6;	//ʱ�������

/**
  * @brief  ��RTCʵʱʱ��д��1�ֽ�����
  * @param  command 1�ֽ�������
  * @param  dataByte 1�ֽ�����
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
  * @brief  ��RTCʵʱʱ�Ӷ���1�ֽ�����
  * @param  command 1�ֽ�������
  * @retval 1�ֽ�����
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
  * @brief  ���RTC����
  * @param  ��
  * @retval ��
  */
void RTCSecondClear(void)
{
	RTCWriteByte(0x80, 0x00);
}

/**
  * @brief  ��ȡRTC����
  * @param  ��
  * @retval ����
  */
unsigned char RTCReadSecond(void)
{
	unsigned char second = RTCReadByte(0x81);
	second = second/16*10 + second%16;
	return second;
}

/**
  * @brief  �޸�RTC����
  * @param  second ����
  * @retval ��
  */
void RTCSetSecond(unsigned char second)
{
	second = second/10*16 + second%10;
	RTCWriteByte(0x80, second);
}