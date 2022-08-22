
void Delay(unsigned int xms) //@11.0592Mhz
{
	unsigned char i, j;
	while(xms--)
	{
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}

