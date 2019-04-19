#include <ESP01.h>

ESP01::ESP01(PinName rx, PinName tx, PinName rst): Uart(rx,tx,115200), Rst(rst)
{
  if( rst == NC )
    ResetMode = Soft;
  else
    ResetMode = Hard;
  
  Reset();
  Uart.attach(callback(this,&ESP01::UartIRQ));
  
  ExternalResponse = NULL;
  _keyword = NULL;
  
}

ESP01::~ESP01()
{
  free(_keyword);
  
  ExternalResponse = NULL;
  _keyword = NULL;
  
}

void ESP01::Send(uint8_t *buf)
{
  uint8_t len = strlen((const char*)buf);
  for(uint8_t i=0; i<len; i++)
    Uart.putc((int)*(buf+i));
}

void ESP01::Send(uint8_t *buf, uint16_t len)
{
  for(uint8_t i=0; i<len; i++)
    Uart.putc((int)*(buf+i));
}

uint8_t ESP01::SendCmd(const char *data, const char *rec, uint16_t timeout)
{
  Clear();
  Send((uint8_t*)data);

  timeout /= 10;
  timeout += 1;
  do{
    if( WaitRecive() == OK )
    {
      if( strstr((const char*)RecBuf, (const char*)rec ) != NULL )
      {
        Clear();
        return OK;
      }
      //Clear();
    }
    wait_ms(10);
  }while(timeout--);
  return ERR;
}

uint8_t ESP01::SendData(uint8_t *data, uint8_t *rec, uint16_t timeout)
{
  Clear();
  Send(data);

  //timeout /= 10;
  timeout += 1;
  do{
    if( WaitRecive() == OK && RecCnt!=0 )
    {
      memcpy(rec,RecBuf,( sizeof(rec)>RecCnt ? sizeof(rec) : RecCnt-1 ));
      
      Clear();
      return OK;
    }
    wait_ms(1);
  }while(timeout--);
  return ERR;
}


uint8_t ESP01::ConnectAP(const char *ssid, const char *pawd)
{
  Reset();

  if( SendCmd("AT\r\n", "OK", 5000) != OK )
    if( SendCmd("AT\r\n", "OK", 5000) != OK )
      if( SendCmd("AT\r\n", "OK", 5000) != OK )
        if( SendCmd("AT\r\n", "OK", 5000) != OK )
          return 4;
    
  if( SendCmd("AT+CWMODE?\r\n", "CWMODE:3", 5000) != OK )
  {
    if( SendCmd("AT+CWMODE=3\r\n", "OK", 5000) != OK )
      if( SendCmd("AT+CWMODE=3\r\n", "OK", 5000) != OK )
        return 3;
    if( SendCmd("AT+RST\r\n", "ready", 10000) != OK )
      if( SendCmd("AT+RST\r\n", "ready", 10000) != OK ) 
        return 2;
  }
  if( SendCmd("AT+CWJAP?\r\n", ssid, 5000) != OK )
  {
    char cmd[64];
    sprintf(cmd,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,pawd);

    if( SendCmd(cmd, "GOT IP", 10000) != OK )
      if( SendCmd(cmd, "GOT IP", 10000) != OK )
        return 1;
  }

  return 0;
}

bool ESP01::ConnectIP(void)
{
  if( _protocol != NULL && _ip != NULL && _port != NULL )
  {
    char cmd_buf[48];
    snprintf(cmd_buf, sizeof(cmd_buf), "AT+CIPSTART=\"%s\",\"%s\",%s\r\n", _protocol, _ip, _port);
    return (SendCmd(cmd_buf,"CONNECT",5000) ? true : ( (SendCmd(cmd_buf,"CONNECT",5000) ? true : false) ));
  }
  else
    return false;
}

bool ESP01::ConnectIP(const char* protocol, const char* IP, const char* port)
{
    char cmd_buf[48];
    
    strcpy(_protocol,protocol);
    strcpy(_ip,IP);
    strcpy(_port,port);

	  snprintf(cmd_buf, sizeof(cmd_buf), "AT+CIPSTART=\"%s\",\"%s\",%s\r\n", _protocol, _ip, _port);
    return (SendCmd(cmd_buf,"CONNECT",5000) ? true : ( (SendCmd(cmd_buf,"CONNECT",5000) ? true : false) ));
}

void ESP01::Reset(void)
{
  Clear();
  if( ResetMode == Soft )
    Send((uint8_t*)"+++");
  else
  {
    Rst = 0;
    wait_ms(10);
    Rst = 1;
  }
}

void ESP01::Clear(void)
{
  memset(RecBuf, 0, sizeof(RecBuf));
	RecCnt = 0;
  RecCntPre = 0;
}

uint8_t ESP01::WaitRecive(void)
{
  uint8_t State = ERR;

  if( RecCnt == 0 )
    State = OK;
  if( RecCnt == RecCntPre )
  {
    State = OK;
  }
  RecCntPre = RecCnt;

  return State;
}

uint8_t* ESP01::GetRecive(void)
{
  memset(RecBuf,RecCnt,sizeof(RecBuf));
  return (uint8_t*)RecBuf;
}

void ESP01::UartIRQ(void)
{
  while(Uart.readable())
  {
    if( RecCnt >= REC_BUF_MAXLEN )
      RecCnt = 0;
    RecBuf[RecCnt++] = (uint8_t)Uart.getc(); 
  }


  for(uint8_t i=sizeof(_keyword), j=1;(i>0)&&(j<RecCnt);i--,j++)
    if( RecBuf[RecCnt-j] == *(_keyword+i-1) )
      if( i==1 )
        if( ExternalResponse != NULL )
    {
      if( !Uart.readable() )
      {
        ExternalResponse();
      }
    }
  
}

bool ESP01::GetIPD(uint8_t *data, uint16_t len, uint16_t timeOut)
{
  char *ptrIPD = NULL;
	//timeOut /= 5;
  timeOut += 1;
	do
	{
		if(WaitRecive() == OK)								//如果接收完成
		{
			ptrIPD = strstr((char *)RecBuf, "IPD,");				//搜索“IPD”头
			if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//找到':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
          len = len > REC_BUF_MAXLEN ? REC_BUF_MAXLEN : len ;
          memcpy(data,ptrIPD,len);
          Clear();
					return true;
				}
				else
					return false;
			}
		}
		wait_ms(1);													//延时等待
	} while(timeOut--);
	
	return false;	
}

void ESP01::attachTrigger(const uint8_t *Keyword, void (*fun)(void))
{
  _keyword = (uint8_t*)malloc(sizeof(Keyword)+1);
  memcpy(_keyword,Keyword,sizeof(Keyword));
  //printf("KEY_WORD:%s\r\n",(char*)_keyword);
  ExternalResponse = fun;
}
void ESP01::detachTrigger(void)
{
  free(_keyword);
  _keyword = NULL;
  ExternalResponse = NULL;
}