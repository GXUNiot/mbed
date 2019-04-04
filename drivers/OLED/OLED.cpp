
#include "OLED.h"

OLED::OLED(I2C* i2c):_i2c(i2c)
{
  SetSpeed(5);
  #if 1
	WriteCom(0xAE); //关闭显示
	WriteCom(0x20); //Set Memory Addressing Mode
	WriteCom(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCom(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
	WriteCom(0xa1); //0xa0，X轴正常显示；0xa1，X轴镜像显示
	WriteCom(0xc8); //0xc0，Y轴正常显示；0xc8，Y轴镜像显示
	WriteCom(0x00); //设置列地址低4位
	WriteCom(0x10); //设置列地址高4位
	WriteCom(0x40); //设置起始线地址
	WriteCom(0x81); //设置对比度值
	WriteCom(0x7f); //------
	WriteCom(0xa6); //0xa6,正常显示模式;0xa7，
	WriteCom(0xa8); //--set multiplex ratio(1 to 64)
	WriteCom(0x3F); //------
	WriteCom(0xa4); //0xa4,显示跟随RAM的改变而改变;0xa5,显示内容忽略RAM的内容
	WriteCom(0xd3); //设置显示偏移
	WriteCom(0x00); //------
	WriteCom(0xd5); //设置内部显示时钟频率
	WriteCom(0xf0); //------
	WriteCom(0xd9); //--set pre-charge period//
	WriteCom(0x22); //------
	WriteCom(0xda); //--set com pins hardware configuration//
	WriteCom(0x12); //------
	WriteCom(0xdb); //--set vcomh//
	WriteCom(0x20); //------
	WriteCom(0x8d); //--set DC-DC enable//
	WriteCom(0x14); //------
	WriteCom(0xaf); //打开显示
#else
	WriteCom(0xAE);   //display off
	WriteCom(0x00);	//Set Memory Addressing Mode
	WriteCom(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCom(0x40);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCom(0xb0);	//Set COM Output Scan Direction
	WriteCom(0x81);//---set low column address
	WriteCom(0xff);//---set high column address
	WriteCom(0xa1);//--set start line address
	WriteCom(0xa6);//--set contrast control register
	WriteCom(0xa8);
	WriteCom(0x3f);//--set segment re-map 0 to 127
	WriteCom(0xad);//--set normal display
	WriteCom(0x8b);//--set multiplex ratio(1 to 64)
	WriteCom(0x33);//
	WriteCom(0xc8);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCom(0xd3);//-set display offset
	WriteCom(0x00);//-not offset
	WriteCom(0xd5);//--set display clock divide ratio/oscillator frequency
	WriteCom(0x80);//--set divide ratio
	WriteCom(0xd9);//--set pre-charge period
	WriteCom(0x1f); //
	WriteCom(0xda);//--set com pins hardware configuration
	WriteCom(0x12);
	WriteCom(0xdb);//--set vcomh
	WriteCom(0x40);//0x20,0.77xVcc
//	IIC_Write_Command(0x8d);//--set DC-DC enable
//	IIC_Write_Command(0x14);//
	WriteCom(0xaf);//--turn on oled panel
#endif
  ClearScreen();
}

bool OLED::WriteData(unsigned char byte)
{
  _i2c->start();
  if(!_i2c->write(OLED_ADDRESS))
    return 1;
  if(!_i2c->write(0x40))
    return 1;
  if(!_i2c->write(byte))
    return 1;
  _i2c->stop();
	return 0;
}

bool OLED::WriteCom(unsigned char cmd)
{
  _i2c->start();
  if(!_i2c->write(OLED_ADDRESS))
    return 1;
  if(!_i2c->write(0x00))
    return 1;
  if(!_i2c->write(cmd))
    return 1;
  _i2c->stop();
	return 0;
}

void OLED::Address(unsigned char x, unsigned char y)
{
  WriteCom(0xb0 + x);					//设置行地址
	wait_ms(_speed);
	WriteCom(((y & 0xf0) >> 4) | 0x10);	//设置列地址的高4位
	wait_ms(_speed);
	WriteCom(y & 0x0f);					//设置列地址的低4位
	wait_ms(_speed);
}

void OLED::ClearScreen(void)
{
  unsigned char i = 0, j = 0;
  for(; i < 8; i++)
	{
		WriteCom(0xb0 + i);
		WriteCom(0x10);
		WriteCom(0x00);

		for(j = 0; j < 132; j++)
		{
			WriteData(0x00);
		}
	}
}

void OLED::ClearAt(unsigned char x)
{
  unsigned char i = 0;

	WriteCom(0xb0 + x);
	WriteCom(0x10);
	WriteCom(0x00);

	for(; i < 132; i++)
	{
		WriteData(0x00);
	}

}

void OLED::Dis12864_Pic(const unsigned char *dp)
{
  unsigned char i = 0, j = 0;


	for(; j < 8; j++)
	{
		Address(j, 0);

		for (i = 0; i < 128; i++)
		{
			WriteData(*dp++); //写数据到LCD,每写完一个8位的数据后列地址自动加1
		}
	}
}

void OLED::DisChar16x16(unsigned short x, unsigned short y, const unsigned char *dp)
{
  unsigned short i = 0, j = 0;

	SetSpeed(20);					//i2c速度控制

	for(j = 2; j > 0; j--)
	{
		Address(x, y);

		for (i = 0; i < 16; i++)
		{
			WriteData(*dp++);		//写数据到OLED,每写完一个8位的数据后列地址自动加1
		}

		x++;
	}
}

void OLED::DisString6x8(unsigned char x, unsigned char y, char *fmt, ...)
{
  unsigned char i = 0, ch = 0;
	unsigned char OledPrintfBuf[300];

	va_list ap;
	unsigned char *pStr = OledPrintfBuf;

	va_start(ap,fmt);
	vsprintf((char *)OledPrintfBuf, fmt, ap);
	va_end(ap);

	y += 2;
	SetSpeed(1);							//i2c速度控制

	while(*pStr != '\0')
	{
		ch = *pStr - 32;

		if(y > 126)
		{
			y = 2;
			x++;
		}

		Address(x, y);
		for(i = 0; i < 6; i++)
			WriteData(F6x8[ch][i]);

		y += 6;
		pStr++;
	}
}

  void OLED::DisString8x16(unsigned char x, unsigned char y, char *fmt, ...)
  {
    unsigned char i = 0, ch = 0;
  	unsigned char OledPrintfBuf[300];

  	va_list ap;
  	unsigned char *pStr = OledPrintfBuf;

  	va_start(ap,fmt);
  	vsprintf((char *)OledPrintfBuf, fmt, ap);
  	va_end(ap);

  	y += 2;
  	SetSpeed(1);							//i2c速度控制

  	while(*pStr != '\0')
  	{
  		ch = *pStr - 32;

  		if(y > 128)
  		{
  			y = 2;
  			x += 2;
  		}

  		Address(x, y);
  		for(i = 0; i < 8; i++)
  			WriteData(F8X16[(ch << 4) + i]);

  		Address(x + 1, y);
  		for(i = 0; i < 8; i++)
  			WriteData(F8X16[(ch << 4) + i + 8]);

  		y += 8;
  		pStr++;
  	}
  }

void OLED::ContinuousDis(char *fmt, ...)
{

  static char buf[32][23],bufCnt=0;

  if(bufCnt>=32)
    bufCnt = 0;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char *)buf[bufCnt++], fmt, ap);
	va_end(ap);
  SetSpeed(1);
  for(int i=0; i<6; i++)
  {
    if(bufCnt-i < 0)
    {
      if(buf[bufCnt-i+32][0]!='\0')
        DisString6x8(i,0,&buf[bufCnt-i+32][0]);

    }
    else
    {
      DisString6x8(i,0,&buf[bufCnt-i][0]);

    }
  }

}
