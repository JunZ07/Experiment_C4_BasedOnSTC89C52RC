#include <REGX52.H>

sbit LCD_EN = P2^7;		//使能端，1时数据输入
sbit LCD_RS = P2^6;		//RS控制端，0为命令，1为数据
sbit LCD_WR = P2^5;		//WR控制端，0为写入，1为读出

#define LCD_DATAIO P0	//数据IO，8位
#define INPASSWORDLEN 7 //密码长度

/**
  * @brief  延迟1ms，晶振脉冲11.0592MHz，用于防止电平切换过快出错
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
  * @brief  写一字节命令
  * @param  command 命令
  * @retval 无
  */
void LCDWriteCommand(unsigned char command)
{
	LCD_RS = 0;
	LCD_WR = 0;
	LCD_DATAIO = command;
	LCD_EN = 1;
	LCDDelay1ms();	//虽然LCD_EN变化时间为ns级别，但是指令执行需要40us，这边加这个主要是防指令延时不够，反正测试能用
	LCD_EN = 0;
	LCDDelay1ms();
}

/**
  * @brief  写一字节数据
  * @param  data0 数据
  * @retval 无
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
  * @brief  显示屏初始化，清贫，设置光标，设置写保护等等
  * @param  无
  * @retval 无
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
  * @brief  清空显示屏某行
  * @param  line 行数 1-第一行 2-第二行 3-两行
  * @retval 无
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
  * @brief  设置光标位置
  * @param  line 行数 1或2
  * @param  column 列数 1~14
  * @retval 无
  */
void LCDSetCursor(unsigned char line, unsigned char column)
{
	if (line == 1)	//address 00H~27H
		LCDWriteCommand(0x80 + column - 1);
	else	//address 40H~67H
		LCDWriteCommand(0x80 + column + 40 - 1);
}

/**
  * @brief  显示一个字符
  * @param  line 行数 1或2
  * @param  column 列数 1~14
  * @param  char0 字符
  * @retval 无
  */
void LCDShowChar(unsigned char line, unsigned char column, unsigned char char0)
{
	LCDSetCursor(line, column);
	LCDWriteData(char0);
}

/**
  * @brief  显示一个字符串
  * @param  line 行数 1或2
  * @param  column 列数 1~14
  * @param  String 字符指针，字符串的首地址
  * @retval 无
  */
void LCDShowString(unsigned char Line,unsigned char Column,unsigned char *String)
{
	unsigned char i;
	LCDSetCursor(Line,Column);
	for(i=0;String[i]!='\0';i++)	//一直写到字符串结尾
		LCDWriteData(String[i]);
}

/**
  * @brief  计算X的Y次方
  * @param  X 底数
  * @param  Y 指数
  * @retval 结果 long型
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
	for(i=Length;i>0;i--)	//循环读出num每一位，其中如果读的位数超过num则读出0，并显示
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
  * @brief  显示出已输入的密码
  * @param  Line 行数
  * @param  Column 列数
  * @param  Number 密码
  * @param  PWCount 密码当前位数
  * @retval 无
  */
void LCDShowInPW(unsigned char Line,unsigned char Column,unsigned long Number,unsigned char PWCount)
{
	unsigned char i;
	LCDSetCursor(Line,Column);
	for (i=1; i<=INPASSWORDLEN-PWCount; i++)
		LCDWriteData('X');
	for(i=PWCount; i>0; i--)	//循环读出num每一位，其中如果读的位数超过num则读出0，并显示
		LCDWriteData('0'+Number/LCDPow(10,i-1)%10);
}
