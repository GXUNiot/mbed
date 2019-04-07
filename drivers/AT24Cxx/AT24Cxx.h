
/*****************************************
 * 文件名： AT24Cxx.h
 * 
 * 说明：   
 * 
 * 注意： 
 * 
 * 作者：   汜方(15977877715@qq.com)
 * 修改时间：   2019-04-04
 *****************************************/

#ifndef _AT24CXX_H
#define _AT24CXX_H

/*****************  库  *****************/
#include "mbed.h"


/*****************  类  *****************/
class AT24CXX
{

public:
    /****************************************
     * 函数名： AT24CXX
     * 功能：   构造函数
     * 参数:    
  	 *          
     * 返回值： 
	****************************************/
    AT24CXX(I2C *i2c, uint16_t size, uint8_t hardaddr, uint32_t feq=100000L);
    ~AT24CXX();

    uint8_t read(uint16_t addr, uint8_t *data);
    uint8_t readNow(uint8_t *data);
    uint8_t read(uint16_t addr, uint8_t *dataArray, uint16_t len);

    uint8_t write(uint16_t addr, uint8_t data);
    uint8_t writePage(uint16_t addr, const uint8_t* data, uint8_t len);
    uint8_t write(uint16_t addr, const uint8_t* data, uint16_t len);

    uint16_t getStorageSize(void);
    uint8_t getPageSize(void);
    uint16_t getMaxAddr(void);

private:

    I2C *_i2c;

    uint8_t _pageSize,_hardAddr;
    uint16_t _storageSize,_addrSize;
    uint32_t _feq;
};

#endif	//_AT24CXX_H
