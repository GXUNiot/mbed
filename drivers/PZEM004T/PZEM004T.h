#ifndef _PZEM004T_H
#define _PZEM004T_H

#include "mbed.h"
#include <stdint.h>

/** PZEM004T 交流多功能通讯模块
 *
 *	注意：该模块需要220AC + 5VDC同时供电， 供电不足将无法正常工作
 *	
 *	产品型号: PZEM-004T(V1.0)
 *	A.功能
 *	1.电参数测量功能（电压、电流、有功功率、电量）。
 *	2.电量按键清零功能。
 *	3.掉电数据保存功能(保存掉电前累计电量）。
 *	4.上位机PC显示功能（显示电压、电流、有功功率、电量）。
 *	5.串口通信功能（自带TTL串行接口，可通过转接板与各种终端通信，读取及设置参数）。
 *	
 *	1.功率：测试量程0~22kW
 *         0~10kW以内显示格式0.000~9.999；
 *         10~22kW以内显示格式10.00~22.00。
 *
 *	2.电量：测试量程0~9999kWh 
 *         0~10kWh以内显示格式0.000~9.999；
 *         10~100kWh以内显示格式10.00~99.99；
 *         100~1000kWh以内显示格式100.0~999.9；
 *         1000~9999kWh及以上显示格式1000~9999。
 *
 *	3.电压：测试量程80~260VAC
 *         显示格式110.0~220.0。
 *
 *	4.电流：测试量程0~100A
 *         显示格式00.00~99.99。
 *		   
 * Example:
 * @code
 *	#include "CNiot.h"
 *
 *	//使用 PC_13 脚连接到LED
 *	LedBlink blink(PC_13);
 *
 *	PZEM004T pzem004t(PB_10,PB_11);
 *
 *	int main() {    
 *		
 *		
 *			printf("System is ready\r\n");
 *		while(1) {		
 *				
 *
 *				printf("V:%0.1fv  I:%0.2fA  P:%dW  E:%dWh\r\n",(float)pzem004t.getVoltage()/10,(float)pzem004t.getCurrent()/100,pzem004t.getActivepower(),pzem004t.getElectricity());
 *				wait(1);
 *				
 *			}
 *	}
 *
 * @endcode
 */


#define PZEM004T_OK				0
#define PZEM004T_TIMEOUT_ERROR	-1
#define PZEM004T_CHECK_ERROR	-2
#define PZEM004T_DATA_ERROR		-3

class PZEM004T
{
public:
	
	PZEM004T(PinName tx,PinName rx);

	int readable(void);
	
	int getVoltage(void);
	
	int getCurrent(void);
	
	int getActivepower(void);
	
	int getElectricity(void);
	
	int Test(void);
	
private:
	Serial _serial;
	bool _irqFlag;
	
	unsigned char _buf[10];
	unsigned char _cnt;
	
	void clear(void);
	
	bool check(void);
	void serial_IRQHandler();
	
};

//static unsigned char PZEM004T::_cmd[4][7] = {{0xB0,0xC0,0xA8,0x01,0x01,0x11,0x1A},{0xB1,0xC0,0xA8,0x01,0x01,0x11,0x1B},{0xB2,0xC0,0xA8,0x01,0x01,0x11,0x1C},{0xB3,0xC0,0xA8,0x01,0x01,0x11,0x1D}};


#endif //_PZEM004T_H
