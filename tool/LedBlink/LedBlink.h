
/*****************************************
 * 文件名： LedBlink.h
 * 
 * 说明：   闪灯,使用Ticker定时器定时调用LedBlink::flash()翻转LED状态，可以用于检测MCU运行正常
 * 
 * 注意：   对于循环中运行软件/代码造成的停止运行bug无法反馈，只能指示MCU上电运行正常         
 * 
 * 作者：   汜方(15977877715@qq.com)
 * 修改时间：   2019-02-18
 *****************************************/

#ifndef _LEDBLINK_H
#define _LEDBLINK_H

/*****************  库  *****************/
#include "mbed.h"



/*****************  类  *****************/
class LedBlink
{

public:
    /****************************************
     * 函数名： LedBlink
     * 功能：   构造函数
     * 参数:    pin     LED引脚
  	 *          time    闪烁间隔 单位：s
     * 返回值： NULL
	****************************************/
    LedBlink(PinName pin,float time = 1.0f);


    /****************************************
     * 函数名： run
     * 功能：   启动Led闪烁
     * 参数:    void
     * 返回值： void
	****************************************/
    void run(void);


    /****************************************
     * 函数名： stop
     * 功能：   停止Led闪烁
     * 参数:    void
     * 返回值： void
	****************************************/
    void stop(void);

    operator uint8_t();	


private:

    DigitalOut _pin;
    float _time;
    Ticker _tBlink;

    void _flash(void);

};

#endif	//_LEDBLINK_H
