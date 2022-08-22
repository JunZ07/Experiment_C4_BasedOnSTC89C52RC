/**************************************************
@introduction
Һ����ʾ����������ʾ�ӿ�ģ�飬���ڸ�����ʾ��Ϣ����ʾ
Һ����ʾ���õ���LCD1602
last modify time:20220822
**************************************************/

#ifndef __LCD1602_H__
#define __LCD1602_H__

void LCDInit(void);		//��ʾ����ʼ���������ȳ�ʼ��

//������������ʾ���ַ����ַ������޷���int���з���int��16����
void LCDShowChar(unsigned char line, unsigned char column, unsigned char char0);
void LCDShowString(unsigned char Line,unsigned char Column,unsigned char *String);
void LCDShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCDShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length);
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);

//�ṩ��Control�ĳ��õ���ʾ����ʾ��ʾ��Ϣ�Ľӿ�
void LCDShowClean(unsigned char line);			//���
void LCDPrintCover(void);						//�������
void LCDPrintPlanted(void);						//����°���Ϣ
void LCDPrintCountDown(unsigned char count);	//�������ʱ
void LCDPrintLine(void);						//��ĳ�д�ӡֱ��
void LCDPrintError(void);						//���������Ϣ
//��ʾ������Ĳ�������
void LCDShowInPW(unsigned char Line,unsigned char Column,unsigned long Number,unsigned char PWCount);

#endif