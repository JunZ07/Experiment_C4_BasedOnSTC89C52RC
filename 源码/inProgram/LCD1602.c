#include <REGX52.H>

sbit LCD_EN = P2^7;		//ʹ�ܶˣ�1ʱ��������
sbit LCD_RS = P2^6;		//RS���ƶˣ�0Ϊ���1Ϊ����
sbit LCD_WR = P2^5;		//WR���ƶˣ�0Ϊд�룬1Ϊ����

#define LCD_DATAIO P0	//����IO��8λ
#define INPASSWORDLEN 7 //���볤��

/**
  * @brief  �ӳ�1ms����������11.0592MHz�����ڷ�ֹ��ƽ�л��������
  * @param  
  * @retval 
  */
void LCDDelay1ms()		//@11.0592MHz
{
	unsigned char i, j;
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}

/**
  * @brief  дһ�ֽ�����
  * @param  command ����
  * @retval ��
  */
void LCDWriteCommand(unsigned char command)
{
	LCD_RS = 0;
	LCD_WR = 0;
	LCD_DATAIO = command;
	LCD_EN = 1;
	LCDDelay1ms();	//��ȻLCD_EN�仯ʱ��Ϊns���𣬵���ָ��ִ����Ҫ40us����߼������Ҫ�Ƿ�ָ����ʱ������������������
	LCD_EN = 0;
	LCDDelay1ms();
}

/**
  * @brief  дһ�ֽ�����
  * @param  data0 ����
  * @retval ��
  */
void LCDWriteData(unsigned char data0)
{
	LCD_RS = 1;
	LCD_WR = 0;
	LCD_DATAIO = data0;
	LCD_EN = 1;
	LCDDelay1ms();
	LCD_EN = 0;
	LCDDelay1ms();
}

/**
  * @brief  ��ʾ����ʼ������ƶ�����ù�꣬����д�����ȵ�
  * @param  ��
  * @retval ��
  */
void LCDInit(void)
{
	LCDWriteCommand(0x38);
	LCDWriteCommand(0x0C);
	LCDWriteCommand(0x06);
	LCDWriteCommand(0x01);
}

void LCDShowString(unsigned char Line,unsigned char Column,unsigned char *String);
/**
  * @brief  �����ʾ��ĳ��
  * @param  line ���� 1-��һ�� 2-�ڶ��� 3-����
  * @retval ��
  */
void LCDShowClean(unsigned char line)
{
	if (line == 1)
		LCDShowString(1, 1, "                ");
	else if (line == 2)
		LCDShowString(2, 1, "                ");
	else {
		LCDShowString(1, 1, "                ");
		LCDShowString(2, 1, "                ");
	}
}

/**
  * @brief  ���ù��λ��
  * @param  line ���� 1��2
  * @param  column ���� 1~14
  * @retval ��
  */
void LCDSetCursor(unsigned char line, unsigned char column)
{
	if (line == 1)	//address 00H~27H
		LCDWriteCommand(0x80 + column - 1);
	else	//address 40H~67H
		LCDWriteCommand(0x80 + column + 40 - 1);
}

/**
  * @brief  ��ʾһ���ַ�
  * @param  line ���� 1��2
  * @param  column ���� 1~14
  * @param  char0 �ַ�
  * @retval ��
  */
void LCDShowChar(unsigned char line, unsigned char column, unsigned char char0)
{
	LCDSetCursor(line, column);
	LCDWriteData(char0);
}

/**
  * @brief  ��ʾһ���ַ���
  * @param  line ���� 1��2
  * @param  column ���� 1~14
  * @param  String �ַ�ָ�룬�ַ������׵�ַ
  * @retval ��
  */
void LCDShowString(unsigned char Line,unsigned char Column,unsigned char *String)
{
	unsigned char i;
	LCDSetCursor(Line,Column);
	for(i=0;String[i]!='\0';i++)	//һֱд���ַ�����β
		LCDWriteData(String[i]);
}

/**
  * @brief  ����X��Y�η�
  * @param  X ����
  * @param  Y ָ��
  * @retval ��� long��
  */
long LCDPow(int X,int Y)
{
	unsigned char i;
	long Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}

void LCDShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCDSetCursor(Line,Column);
	for(i=Length;i>0;i--)	//ѭ������numÿһλ�������������λ������num�����0������ʾ
		LCDWriteData('0'+Number/LCDPow(10,i-1)%10);
}

void LCDShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length)
{
	unsigned char i;
	unsigned int Number1;
	LCDSetCursor(Line,Column);
	if(Number>=0)
	{
		LCDWriteData('+');
		Number1=Number;
	}
	else
	{
		LCDWriteData('-');
		Number1=-Number;
	}
	for(i=Length;i>0;i--)
	{
		LCDWriteData('0'+Number1/LCDPow(10,i-1)%10);
	}
}

void LCDPrintCover(void)
{
	LCDShowString(1, 1, "PASSWORD:       ");
	LCDShowString(2, 1, "XXXXXXX         ");
}

void LCDPrintPlanted(void)
{
	LCDShowString(1, 1, "The C4 bomb has ");
	LCDShowString(2, 1, "been planted!   ");
}

void LCDPrintCountDown(unsigned char count)
{
	LCDShowString(1, 1, "COUNTDOWN:      ");
	LCDShowNum(1, 11, count, 2);
}
void LCDPrintLine(void)
{
	LCDShowString(1, 1, "----------------");
	LCDShowString(2, 1, "----------------");
}

void LCDPrintError(void)
{
	LCDShowString(2, 1, "ERROR!          ");
}

void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i,SingleNumber;
	LCDSetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		SingleNumber=Number/LCDPow(16,i-1)%16;
		if(SingleNumber<10)
		{
			LCDWriteData(SingleNumber+'0');
		}
		else
		{
			LCDWriteData(SingleNumber-10+'A');
		}
	}
}

/**
  * @brief  ��ʾ�������������
  * @param  Line ����
  * @param  Column ����
  * @param  Number ����
  * @param  PWCount ���뵱ǰλ��
  * @retval ��
  */
void LCDShowInPW(unsigned char Line,unsigned char Column,unsigned long Number,unsigned char PWCount)
{
	unsigned char i;
	LCDSetCursor(Line,Column);
	for (i=1; i<=INPASSWORDLEN-PWCount; i++)
		LCDWriteData('X');
	for(i=PWCount; i>0; i--)	//ѭ������numÿһλ�������������λ������num�����0������ʾ
		LCDWriteData('0'+Number/LCDPow(10,i-1)%10);
}
