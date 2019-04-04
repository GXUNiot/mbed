#include "WS2812.h"

WS2812::WS2812(PinName pin, uint8_t num):_Pin(pin)
{
    _LedNum = num;
    _LedDataBuf = (uint8_t*)malloc(sizeof(uint8_t)*3*_LedNum);
    if(_LedDataBuf == NULL)
    {
        printf("Error\r\nWS2812 Init---malloc fail\r\n");
    }
    _Pin = 1;
}

void WS2812::write(uint8_t r, uint8_t g, uint8_t b)
{
    for(uint8_t index=0; index<_LedNum; index++)
    {
        _LedDataBuf[index*3] = g;
        _LedDataBuf[index*3+1] = r;
        _LedDataBuf[index*3+2] = b;
    }
    WS2812::show();
}

void WS2812::write(uint32_t color)
{
    for(uint8_t index=0; index<_LedNum; index++)
    {
        _LedDataBuf[index*3] = (uint8_t)((color&0x00FF00)>>8);
        _LedDataBuf[index*3+1] = (uint8_t)((color&0xFF0000)>>16);
        _LedDataBuf[index*3+2] = (uint8_t)((color&0x0000FF));
    }
    WS2812::show();
}

void WS2812::write(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    _LedDataBuf[index*3] = g;
    _LedDataBuf[index*3+1] = r;
    _LedDataBuf[index*3+2] = b;
    WS2812::show();
}

void WS2812::write(uint8_t index, uint32_t color)
{
    _LedDataBuf[index*3] = (uint8_t)((color&0x00FF00)>>8);
    _LedDataBuf[index*3+1] = (uint8_t)((color&0xFF0000)>>16);
    _LedDataBuf[index*3+2] = (uint8_t)((color&0x0000FF));
    WS2812::show();
}

void WS2812::write(uint8_t num, uint8_t* color)
{
    for(uint8_t index=0; (index<num)&&(index<_LedNum); index++)
    {
        _LedDataBuf[index*3] = color[index*3+1];
        _LedDataBuf[index*3+1] = color[index*3];
        _LedDataBuf[index*3+2] = color[index*3+2];
    }
    WS2812::show();
}

void WS2812::clean(void)
{
    memset(_LedDataBuf,0,_LedNum);
    WS2812::show();
}

void WS2812::show(void)
{
    for(uint8_t index=0; index<_LedNum; index++)
    {
        for(uint8_t piex=0; piex<3; piex++)
        {
            for(uint8_t bite=0; bite<8; bite++)
            {
                if( _LedDataBuf[(index*3)+piex] & (0x01<<bite) )
                    WS2812::_One();
                else
                    WS2812::_Zero();
            }
        }
    }
}

void WS2812::close(void)
{
    for(uint8_t index=0; index<_LedNum; index++)
    {
        for(uint8_t piex=0; piex<3; piex++)
        {
            for(uint8_t bite=0; bite<8; bite++)
            {
                WS2812::_Zero();
            }
        }
    }
}

void WS2812::test(void)
{
    static Timer timer;
    static uint32_t random_attach,timer_cnt;
    timer.start();
    
    for(uint8_t index=0; index<_LedNum; index++)
    {
        if( timer.read() != timer_cnt )
        {
            timer_cnt = timer.read();
            srand(timer.read()+random_attach++);
            uint32_t color = (rand() % 0xFFFFFF);
            WS2812::write(index,color);   
        }
    }
}

void WS2812::PoliceLight(float second, uint8_t mode)
{
    static Ticker ticker;
    switch(mode)
    {
        case POLICE_LIGHT_FLASH:
            ticker.attach(callback(this,&WS2812::_Police_Light_Flash),second);
            break;
        case POLICE_LIGHT_FLOW:

            break;
        default:
            ticker.detach();
            break;
    }
}

void WS2812::_Police_Light_Flash(void)
{
    static uint8_t flag;
    if( flag++ % 2 )
    {
        for(uint8_t index=0; index<(_LedNum/2); index++)
        {
            WS2812::write(index,(uint32_t)RED);
            WS2812::write(_LedNum-index,(uint32_t)0x000000);
        }
    }
    else
    {
        for(uint8_t index=0; index<(_LedNum/2); index++)
        {
            WS2812::write(index,(uint32_t)0x000000);
            WS2812::write(_LedNum-index,(uint32_t)BLUE);
        }
    }
}
void WS2812::_Police_Light_Flow(void)
{

}

void WS2812::_Zero(void)
{
    //H 220ns~380ns
    for(uint16_t cnt=0; cnt<4; cnt++)
        _Pin = 1;

    //L 750ns~1.6us
    for(uint16_t cnt=0; cnt<6; cnt++)
        _Pin = 0;
}
void WS2812::_One(void)
{
    //H 750ns~1.6us
    for(uint16_t cnt=0; cnt<8; cnt++)
        _Pin = 1;

    //L 220ns~420ns
    _Pin = 0;
}

void WS2812::_Res(void)
{
    _Pin = 0;
    wait_us(300);
}