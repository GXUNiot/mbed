/*
#include <mbed.h>
#include "LedBlink.h"
#include "AT24Cxx.h"

LedBlink LED(PC_13);
Serial DEBUG(PA_2,PA_3,115200);




I2C i2cBus(PB_7,PB_6);

#define TEST_DATA_SIZE 1024
uint8_t *TestData;


int main() {
  // put your setup code here, to run once:

  //准备测试数据
  TestData = (uint8_t*)malloc(TEST_DATA_SIZE);
  for(uint16_t i=0; i<TEST_DATA_SIZE; i++)
    *(TestData+i) = (uint8_t)i;


  wait(3);

  AT24CXX EEPROM(&i2cBus, 32, 7);

  //EEPROM 信息
  DEBUG.printf("EEPROM storage size:%dKB \r\n",EEPROM.getStorageSize());
  DEBUG.printf("EEPROM page size:%dByte \r\n",EEPROM.getPageSize());
  DEBUG.printf("EEPROM addr size:%d \r\n\n",EEPROM.getMaxAddr());

  uint16_t addr = 0;

  //连续单次读写测试
  DEBUG.printf("EEPROM one W/R Test Start\r\n");
  for(uint16_t i=0; i<TEST_DATA_SIZE; i++)
  {
    DEBUG.printf(".");
    uint8_t result = EEPROM.write(addr,*(TestData+i));
    if( result != 0 )
      DEBUG.printf("EEPROM one write Err:%d-%d\r\n",addr,result);
    wait_ms(10);

    uint8_t data=0;
    result = EEPROM.read(addr,&data);
    if( result != 0 )
      DEBUG.printf("EEPROM one read Err:%d-%d\r\n",addr,result);
    else
      if( data != *(TestData+i) )
        DEBUG.printf("EEPROM one check Err:%d-%2x-%2x\r\n",i,*(TestData+i),data);
    addr++;
  }

  wait_ms(500);

  //连续页写读取测试
  DEBUG.printf("EEPROM page W/R Test Start\r\n");
  uint8_t pageSize = EEPROM.getPageSize();
  for(uint16_t i=1; i<=pageSize; i++)
  {
    DEBUG.printf(".");
    uint8_t result = EEPROM.writePage(addr,TestData,i);
    if( result != 0 )
      DEBUG.printf("EEPROM page write Err:%d-%d\r\n",i,result);
    wait_ms(10);
    uint8_t* data = (uint8_t*)malloc(i);
    result = EEPROM.read(addr,data,i);
    if( result != 0 )
      DEBUG.printf("EEPROM page read Err:%d-%d\r\n",i,result);
    else
      for(uint16_t j=0; j<i; j++)
        if( *(data+j) != *(TestData+j) )
          DEBUG.printf("EEPROM page check Err Time:%d-Index:%d-%2x-%2x\r\n",i,j,*(TestData+j),*(data+j));
    free(data);
    data = NULL;
    addr+=i;
  }

  wait_ms(500);

  
  //连续大量读写测试
  DEBUG.printf("EEPROM huge W/R Test Start\r\n");
  addr = 0;
  uint16_t addrSize = EEPROM.getMaxAddr();
  for(uint16_t i=1; i <= addrSize/TEST_DATA_SIZE; i++)
  {
    DEBUG.printf(".");
    uint8_t result = EEPROM.write(addr,TestData,TEST_DATA_SIZE);
    if( result != 0 )
      DEBUG.printf("EEPROM huge write Err:%d-%d\r\n",i,result);
    wait_ms(20);
    uint8_t* data = (uint8_t*)malloc(TEST_DATA_SIZE);
    result = EEPROM.read(addr,data,TEST_DATA_SIZE);
    if( result != 0 )
      DEBUG.printf("EEPROM huge read Err:%d-%d\r\n",i,result);
    else
      for(uint16_t j=0; j<TEST_DATA_SIZE; j++)
        if( *(data+j) != *(TestData+j) )
          DEBUG.printf("EEPROM huge check Err Time:%d-Index:%d-%2x-%2x\r\n",i,j,*(TestData+j),*(data+j));
    free(data);
    data = NULL;
    addr+=TEST_DATA_SIZE;
  }


  DEBUG.printf("Sys is ready\r\n");
  while(1) {
    // put your main code here, to run repeatedly:
  }
}
*/