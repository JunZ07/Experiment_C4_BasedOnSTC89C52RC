/**************************************************
@introduction
RTCʵʱʱ��DS1302������ģ��
��Ҫ���ڻ�ȡʵʱ׼ȷ�ĵ���ʱ
last modify time: 20220822am
**************************************************/

#ifndef __DS1302_H__
#define __DS1302_H__

//void RTCWriteByte(unsigned char command, dataByte);
//unsigned char RTCReadByte(unsigned char command);
void RTCSecondClear(void);					//���RTC����
unsigned char RTCReadSecond(void);			//����RTC����
void RTCSetSecond(unsigned char second);	//��ȡRTC����


#endif