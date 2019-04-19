/*****************************************
 * 文件名：   OneNET.h
 * 
 * 说明：    
 * 
 * 
 * 作者：    汜方(15977877715@qq.com)
 * 修改时间：  2019-03-23
 *****************************************/

#ifndef _ONENET_H
#define _ONENET_H

/***************** 库 *****************/
#include <mbed.h>
#include "typedef.h"
#include "ESP01.h"

#include <string.h>
#include <stdio.h>

extern "C"{
#include "edp/edpkit.h"
#include "dataStream/dStream.h"
}

/***************** 测试 *****************/


/***************** 定义 *****************/

enum ONENET_PROTOCOL{HTTP,EDP,MQTT};

class OneNET
{
private:
    char _devid[16],_apikey[32];
    unsigned char dataMem[64];		//全局数组方式
    ESP01 *_esp8266;
    ONENET_PROTOCOL _protocol;
    bool Send(uint8_t* data);
    bool Send(uint8_t* data, uint16_t len);
public:
    OneNET(const char* devid, const char* apikey);
    ~OneNET();

    bool DevLink(ESP01 *esp8266, ONENET_PROTOCOL type);
    bool PushData(const char* dst_devid, const char* data, uint8_t len);
    bool SendData(DATA_STREAM *streamArray, uint8_t streamArrayCnt);
    void RevPro(unsigned char *cmd,char* order, int* value);
};



/***************** 类 *****************/



#endif //_ONENET_H
