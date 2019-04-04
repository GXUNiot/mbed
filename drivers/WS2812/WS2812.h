/*************************************************************************************
*   ***** WS2812 *****
*   
*   说明：用于驱动WS2812 RGB灯带
*
*
*   作者：汜方
*   最后修改时间：2018-11-28
*************************************************************************************/

#ifndef _WS2812_H
#define _WS2812_H

#include "mbed.h"
#include <stdlib.h>

#define RED         0xFF0000
#define GREEN       0x00FF00
#define BLUE        0x0000FF
#define YELLOW      0xEEEE00
#define WHITE       0xFFFFFF


#define POLICE_LIGHT_FLASH      0x01
#define POLICE_LIGHT_FLOW       0x02

class WS2812
{
public:
    WS2812(PinName pin, uint8_t num=30);

    void write(uint8_t r, uint8_t g, uint8_t b);

    void write(uint32_t color);

    void write(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

    void write(uint8_t index, uint32_t color);

    void write(uint8_t num, uint8_t* color);

    void clean(void);

    void show(void);

    void close(void);

    void test(void);

    void PoliceLight(float second=0.1f, uint8_t mode=POLICE_LIGHT_FLASH);

private:
    DigitalOut  _Pin;
    uint8_t     _LedNum;
    uint8_t*    _LedDataBuf;

    void _Zero(void);
    void _One(void);
    void _Res(void);

    void _Police_Light_Flash(void);
    void _Police_Light_Flow(void);
};

#endif //_WS2812_H