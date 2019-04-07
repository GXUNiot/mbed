#include "AT24Cxx.h"

AT24CXX::AT24CXX(I2C *i2c, uint16_t size, uint8_t hardaddr, uint32_t feq)
{

  AT24CXX::_i2c = i2c;
  uint16_t i;
  for(i=512; i>=0x01; i>>=1)
  {
    if( size == i )
    {
      
      break;
    }
  }
  _storageSize = i;
  

  if( _storageSize == 1 )
  {
    _pageSize = 8;
  }
  else if( _storageSize <= 16 )
  {
    _pageSize = 16;
  }
  else if( _storageSize <= 64 )
  {
    _pageSize = 32;
  }
  else if( _storageSize <= 256 )
  {
    _pageSize = 64;
  }

  _addrSize = i*128;

  _feq = feq;

  _hardAddr = (hardaddr & 0x7)<<1;

  _i2c = NULL;
}

AT24CXX::~AT24CXX()
{
  _i2c = NULL;
}

uint8_t AT24CXX::read(uint16_t addr, uint8_t *data)
{
  //检查地址有效性
  if( addr >= _addrSize )                
    return 5;

  (*_i2c).frequency(_feq);
  (*_i2c).start();

  //伪写
  if( (*_i2c).write(0xA0|_hardAddr) != 1 )
  {
    wait_ms(10);
    (*_i2c).start();
    if( (*_i2c).write(0xA0|_hardAddr) != 1 ) 
    {
      (*_i2c).stop();
      return 4;
    }
  }

  //如果是长地址
  if( _addrSize > 0xFF )   //发送高位地址             
    if( (*_i2c).write(addr >> 8) != 1 ) {(*_i2c).stop();return 3;}

  //发送低位地址
  if( (*_i2c).write(addr & 0xFF) != 1 ) {(*_i2c).stop();return 2;}

  //读命令
  (*_i2c).start();
  if( (*_i2c).write(0xA1 | _hardAddr ) != 1 ) {(*_i2c).stop();return 1;}

  *data = (*_i2c).read(0);

  //操作完成
  (*_i2c).stop();
  return 0;
  
}

uint8_t AT24CXX::readNow(uint8_t *data)
{
  (*_i2c).frequency(_feq);
  (*_i2c).start();

  //读命令
  if( (*_i2c).write(0xA1|_hardAddr) != 1 )
  {
    wait_ms(10);
    (*_i2c).start();
    if( (*_i2c).write(0xA1|_hardAddr) != 1 ) 
    {
      (*_i2c).stop();
      return 1;
    }
  }

  *data = (*_i2c).read(0);

  //操作完成
  (*_i2c).stop();
  return 0;
}

uint8_t AT24CXX::read(uint16_t addr, uint8_t *dataArray, uint16_t len)
{
  //检查地址有效性
  if( addr >= _addrSize || (addr + len) > _addrSize )
    return 5;

  (*_i2c).frequency(_feq);
  (*_i2c).start();

  //伪写
  if( (*_i2c).write(0xA0|_hardAddr) != 1 )
  {
    wait_ms(10);
    (*_i2c).start();
    if( (*_i2c).write(0xA0|_hardAddr) != 1 ) 
    {
      (*_i2c).stop();
      return 4;
    }
  }

  //如果是长地址
  if( _addrSize > 0xFF )   //发送高位地址             
    if( (*_i2c).write(addr >> 8) != 1 ) {(*_i2c).stop();return 3;}

  //发送低位地址
  if( (*_i2c).write(addr & 0xFF) != 1 ) {(*_i2c).stop();return 2;}

  //读命令
  (*_i2c).start();
  if( (*_i2c).write(0xA1 | _hardAddr ) != 1 ) {(*_i2c).stop();return 1;}

  //连续读取
  for(; len >0; len--,dataArray++)
    if(len!=1)
      *dataArray = (*_i2c).read(1);
    else
      *dataArray = (*_i2c).read(0);

  //操作完成
  (*_i2c).stop();
  return 0;
}

uint8_t AT24CXX::write(uint16_t addr, uint8_t data)
{

  //检查地址有效性
  if( addr >= _addrSize )                
    return 5;

  (*_i2c).frequency(_feq);
  (*_i2c).start();

  //写命令
  if( (*_i2c).write(0xA0|_hardAddr) != 1 )
  {
    wait_ms(10);
    (*_i2c).start();
    if( (*_i2c).write(0xA0|_hardAddr) != 1 ) 
    {
      (*_i2c).stop();
      return 4;
    }
  }
  //如果是长地址
  if( _addrSize > 0xFF )   //发送高位地址             
    if( (*_i2c).write(addr >> 8) != 1 ) {(*_i2c).stop();return 3;}

  //发送低位地址
  if( (*_i2c).write(addr & 0xFF) != 1 ) {(*_i2c).stop();return 2;}

  //发送数据
  if( (*_i2c).write(data & 0xFF) != 1 ) {(*_i2c).stop();return 1;}

  //操作完成
  (*_i2c).stop();
  return 0;
  
}

uint8_t AT24CXX::writePage(uint16_t addr, const uint8_t* data, uint8_t len)
{

  //检查地址有效性
  if( addr >= _addrSize )                
    return 7;
  if( (addr + len) > _addrSize )                
    return 6;
  if( len > _pageSize )                
    return 5;

  //不是整页地址
  if( (addr % _pageSize) != 0 )
  {
    //计算当前页剩余地址
    uint8_t shortlen = _pageSize-(addr % _pageSize);
    //写入长度超剩余地址
    if( len > shortlen )
    {
      //拆分写入页
      uint8_t result = writePage(addr,data,shortlen);
      if( result != 0 )
        return (10+result);
      addr += shortlen;
      data += shortlen;
      len -= shortlen;
      wait_ms(10);
    }
  }

  (*_i2c).frequency(_feq);
  (*_i2c).start();

  //写命令
  if( (*_i2c).write(0xA0|_hardAddr) != 1 )
  {
    wait_ms(10);
    (*_i2c).start();
    if( (*_i2c).write(0xA0|_hardAddr) != 1 ) 
    {
      (*_i2c).stop();
      return 4;
    }
  }
  
  //如果是长地址
  if( _addrSize > 0xFF )   //发送高位地址             
    if( (*_i2c).write(addr >> 8) != 1 ) {(*_i2c).stop();return 3;}

  //发送低位地址
  if( (*_i2c).write(addr & 0xFF) != 1 ) {(*_i2c).stop();return 2;}

  for( ; len>0; len--,data++)
      if( (*_i2c).write((*data) & 0xFF) != 1 ) {(*_i2c).stop();return 1;}

  //操作完成
  (*_i2c).stop();
  return 0;
  
}

uint8_t AT24CXX::write(uint16_t addr, const uint8_t* data, uint16_t len)
{
  //检查地址有效性
  if( addr >= _addrSize )                
    return 6;
  if( (addr + len) > _addrSize )   
    return 5;

  uint8_t count = 1;
  while( len > _pageSize )
  {
    //printf("addr:%d\rlen:%d",addr,len);
    uint8_t result = writePage(addr,data,_pageSize);
    if( result != 0 )
      return (count*10+result);
    count++;
    addr += _pageSize;
    data += _pageSize;
    len -= _pageSize;
    wait_ms(10);
  };
  //printf("addr:%d\rlen:%d",addr,len);
  
  return writePage(addr,data,len);
}

uint16_t AT24CXX::getStorageSize(void)
{
  return AT24CXX::_storageSize;
}

uint8_t AT24CXX::getPageSize(void)
{
  return AT24CXX::_pageSize;
}

uint16_t AT24CXX::getMaxAddr(void)
{
  return AT24CXX::_addrSize;
}