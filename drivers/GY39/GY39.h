#ifndef _GY39_H 
#define _GY39_H

#include "mbed.h"
#include <stdint.h>

#define CHECK_OK 		0
#define CHECK_ERROR 	1


/** GY39 环境监测气象站模块
 *
 *	1.芯片型号：BME280/MAX44009/STM32F030XXXX
 *	2.工作电压：3～5V
 *	3.工作电流：5mA
 *	4.工作温度：-40℃～+85℃
 *	5.温度范围：-40℃～+85℃
 *	6.湿度范围：0%～100%
 *	7.光强范围：0.045lux～188000lux
 *	8.气压范围：300～1100hpa
 *	9.响应频率：10HZ
 * Example:
 * @code
 *	#include "CNiot.h"
 *
 *	//使用 PC_13 脚连接到LED
 *	LedBlink blink(PC_13);
 *
 *	GY39 gy39(PB_10,PB_11);
 *
 *	int main() {    
 *		printf("System is ready\r\n");
 *
 *		while(1) {		
 *			if(gy39.read())
 *				printf("L:%d  P:%0.2f  T:%0.2f  H:%0.2f  A:%d\r\n",gy39.getLux(),gy39.getPressure(),gy39.getTemp(),gy39.getHum(),gy39.getAlt());
 *						
 *			wait(0.2);
 *		}
 *	}
 *
 * @endcode
 */


class GY39
{
public:
	GY39(PinName tx, PinName);
	
	bool read(void);
	
	int getLux(void);
	
	float getPressure(void);
	
	float getTemp(void);
	
	float getHum(void);
	
	int getAlt(void);
	
private:
	bool readflag;
	Serial _serial;
	unsigned char _buf[16];
	unsigned char _counter;
	enum DATATYPE{Lux,Other,Wait,Lux_OK,Other_OK,Error}_dataType;
	
	int _Lux;
	float _P;
	float _Temp;
	float _Hum;
	float _Alt;

	void Serial_IRQHandler();
};

#endif //_GY39_H
