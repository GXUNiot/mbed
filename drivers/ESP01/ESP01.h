/*****************************************
 * 文件名：   ESP01.h
 * 
 * 说明：    
 * 
 * 
 * 作者：    汜方(15977877715@qq.com)
 * 修改时间：  2019-02-23
 *****************************************/

#ifndef _ESP01_H
#define _ESP01_H

/***************** 库 *****************/
#include <mbed.h>
#include "typedef.h"

/***************** 测试 *****************/


/***************** 定义 *****************/


#define OK 1
#define ERR 0

#define REC_BUF_MAXLEN 128


/***************** 类 *****************/
class ESP01
{

public:

  ESP01(PinName rx, PinName tx, PinName rst=NC);

  ~ESP01();

  void Send(uint8_t *buf);

  void Send(uint8_t *buf, uint16_t len);

  uint8_t SendCmd(const char *data, const char *rec, uint16_t timeout=2000);

  uint8_t SendData(uint8_t *data, uint8_t *rec, uint16_t timeout=2000);

  uint8_t ConnectAP(const char *ssid, const char *pawd);

  bool ConnectIP(void);
  bool ConnectIP(const char* protocol, const char* IP, const char* port);

  void Reset(void);

  uint8_t WaitRecive(void);

  uint8_t* GetRecive(void);

  void Clear(void);

  bool GetIPD(uint8_t *data, uint16_t len, uint16_t timeOut=1000);

  void attachTrigger(const uint8_t *Keyword, void (*fun)(void));
  void detachTrigger(void);

private:

  Serial Uart;
  DigitalOut Rst;

  enum ESP01_RESET_MODE{Soft=0,Hard};
  ESP01_RESET_MODE ResetMode;

  uint8_t RecBuf[REC_BUF_MAXLEN];
  uint16_t RecCnt,RecCntPre;

  uint8_t *_keyword;
  void (*ExternalResponse)(void);

  char _protocol[4],_ip[17],_port[6];

  void UartIRQ(void);
};


#endif //_ESP01_H
