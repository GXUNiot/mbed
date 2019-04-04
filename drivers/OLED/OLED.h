#ifndef _OLED_H
/*
*   ***** 闪灯 *****
*	  备注：使用Ticker定时器定时调用LedBlink::flash()翻转LED状态，可以用于检测MCU运行正常
*	  注意：对于循环中运行软件/代码造成的停止运行bug无法反馈，只能指示MCU上电运行正常
*
*   作者：汜方
*   2019-03-19
*/

#define _OLED_H

#include "mbed.h"
#include "oled_zk.h"

#define OLED_ADDRESS 0x78

class OLED
{

public:
  OLED(I2C* i2c);

  /*
  *   函数名：  WriteData
  *   功能：    写入一个数据
  *
  *   入口参数：（unsigned char）byte---需要写入的数据
  *
  *   返回值：  0-成功
  *            1-失败
  */
  bool WriteData(unsigned char byte);

  /*
  *   函数名：  WriteCom
  *   功能：    写入一个命令
  *
  *   入口参数：（unsigned char）cmd---需要写入的参数
  *
  *   返回值：  0-成功
  *            1-失败
  */
  bool WriteCom(unsigned char cmd);

  /*
  *   函数名：  Address
  *   功能：    设置OLED显示地址
  *
  *   入口参数：（unsigned char）x---行地址
  *             (unsigned char) y---列地址
  *
  *   返回值：  无
  */
  void Address(unsigned char x, unsigned char y);

  /*
  *   函数名：  ClearScreen
  *   功能：    OLED全屏清除
  *
  *   入口参数：无
  *
  *   返回值：  无
  */
  void ClearScreen(void);

  /*
  *   函数名：  ClearAt
  *   功能：    OLED清除指定行
  *
  *   入口参数：x：需要清除的行
  *
  *   返回值：  无
  */
  void ClearAt(unsigned char x);

  /*
  *   函数名：  ClearAt
  *   功能：    OLED清除指定行
  *
  *   入口参数：x：需要清除的行
  *
  *   返回值：  无
  */
  void Dis12864_Pic(const unsigned char *dp);

  /*
  *   函数名：  DisChar16x16
  *   功能：    显示16x16的点阵数据
  *
  *   入口参数：dp：图片数据指针
  *
  *   返回值：  无
  */
  void DisChar16x16(unsigned short x, unsigned short y, const unsigned char *d);

  /*
  *   函数名：  DisString6x8
  *   功能：    显示6x8的点阵数据
  *
  *   入口参数：x：显示行
  *            y：显示列
  *            fmt：不定长参
  *
  *   返回值：  无
  */
  void DisString6x8(unsigned char x, unsigned char y, char *fmt, ...);

  /*
  *   函数名：  DisString8x16
  *   功能：    显示8x16的点阵数据
  *
  *   入口参数：x：显示行
  *            y：显示列
  *            fmt：不定长参
  *
  *   返回值：  无
  */
  void DisString8x16(unsigned char x, unsigned char y, char *fmt, ...);

  /*
  *   函数名：  ContinuousDis
  *   功能：    连续显示，显示8x16的点阵数据
  *
  *   入口参数：fmt：不定长参
  *
  *   返回值：  无
  */
  void ContinuousDis(char *fmt, ...);

  /*
  *   函数名：  SetSpeed
  *   功能：    设置I2C频率
  *
  *   入口参数：x----I2C延时长度-us
  *
  *   返回值：  无
  */
#define SetSpeed(x)   _i2c->frequency(1000000/x);_speed=x;

  /*
  *   函数名：  read
  *   功能：    读取数据
  *
  *   入口参数： (int)address----地址
  *             (char)*data------指向字节数组以读取数据的指针
  *             (int)length-----要读取的字节数
  *
  *   返回值：  0成功（ack）
  *            非0失败（nack）
  */
  //int 	read (int address, char *data, int length, bool repeated=false);

  /*
  *   函数名：  read
  *   功能：    从I2C总线读取一个字节
  *
  *   入口参数：ACK	指示该字节是否被确认（1 =确认）
  *
  *   返回值：  读取的字节
  */
  //int 	read (int ack);

  /*
  *   函数名：
  *   功能：   (int)address----地址
  *
  *   入口参数：(char)*data----指向要发送的字节数组数据的指针
  *            (int)length----要发送的字节数
  *            (bool)repeated-重复开始
  *
  *   返回值：  0成功（ack）
  *            非0失败（nack）
  */
  //int 	write (int address, const char *data, int length, bool repeated=false);

  /*
  *   函数名：  write
  *   功能：   在I2C总线上写入单个字节
  *
  *   入口参数：（int）data---要发送的字节
  *
  *   返回值：  0成功（ack）
  *            非0失败（nack）
  */
  //int 	write (int data);

  /*
  *   函数名：  start
  *   功能：   在I2C总线上创建一个启动条件
  *
  *   入口参数：无
  *
  *   返回值：  无
  */
  //void 	start (void);

  /*
  *   函数名：  stop
  *   功能：   在I2C总线上创建一个停止条件
  *
  *   入口参数：无
  *
  *   返回值：  无
  */
  //void 	stop (void);



private:
  I2C* _i2c;
  unsigned short _speed;
};

#endif //_OLED_H
