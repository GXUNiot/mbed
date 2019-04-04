#include "OneNET.h"

OneNET::OneNET(char* devid, char* apikey)
{
  strcpy(_devid,devid);
  strcpy(_apikey,apikey);
}

OneNET::~OneNET()
{
}

bool OneNET::DevLink(ESP8266 *esp8266, ONENET_PROTOCOL type)
{
  _esp8266 = esp8266; 
  _protocol = type;

  	
	EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};				//协议包

	unsigned char *dataPtr;
	
	unsigned char status = 1;
	
	DEBUG.printf("OneNet_DevLink DEVID: %s,     APIKEY: %s\r\n", _devid, _apikey);

	if(EDP_PacketConnect1(_devid, _apikey, 256, &edpPacket) == 0)		//根据devid 和 apikey封装协议包
	{
		Send(edpPacket._data, edpPacket._len);			//上传平台
		
		dataPtr = (*_esp8266).GetIPD(250);								//等待平台响应
		if(dataPtr != NULL)
		{
			if(EDP_UnPacketRecv(dataPtr) == CONNRESP)
			{
				switch(EDP_UnPacketConnectRsp(dataPtr))
				{
					case 0:DEBUG.printf("Tips:	连接成功\r\n");status = 0;break;
					
					case 1:DEBUG.printf("WARN:	连接失败：协议错误\r\n");break;
					case 2:DEBUG.printf("WARN:	连接失败：设备ID鉴权失败\r\n");break;
					case 3:DEBUG.printf("WARN:	连接失败：服务器失败\r\n");break;
					case 4:DEBUG.printf("WARN:	连接失败：用户ID鉴权失败\r\n");break;
					case 5:DEBUG.printf("WARN:	连接失败：未授权\r\n");break;
					case 6:DEBUG.printf("WARN:	连接失败：授权码无效\r\n");break;
					case 7:DEBUG.printf("WARN:	连接失败：激活码未分配\r\n");break;
					case 8:DEBUG.printf("WARN:	连接失败：该设备已被激活\r\n");break;
					case 9:DEBUG.printf("WARN:	连接失败：重复发送连接请求包\r\n");break;
					
					default:DEBUG.printf("ERR:	连接失败：未知错误\r\n");break;
				}
			}
		}
		
		EDP_DeleteBuffer(&edpPacket);								//删包
    (*_esp8266).Clear();
	}
	else
		DEBUG.printf("WARN:	EDP_PacketConnect Failed\r\n");
	
	return status;

}

bool OneNET::Send(uint8_t* data)
{
  char cmd_buf[40];
  uint16_t len = sizeof(data);

  sprintf(cmd_buf, "AT+CIPSEND=%d\r\n", len);		//发送命令
  if( (*_esp8266).SendCmd(cmd_buf, ">") )
  {
    (*_esp8266).Send(data,len);
    return true;
  }
  return false;
}

bool OneNET::Send(uint8_t* data, uint16_t len)
{
  char cmd_buf[40];

  sprintf(cmd_buf, "AT+CIPSEND=%d\r\n", len);		//发送命令
  if( (*_esp8266).SendCmd(cmd_buf, ">") )
  {
    (*_esp8266).Send(data,len);
    return true;
  }
  return false;
}

bool OneNET::PushData(const char* dst_devid, const char* data)
{
	return false;
}

bool OneNET::SendData(DATA_STREAM *streamArray, uint8_t streamArrayCnt)
{
  if( _protocol == HTTP )
  {

  }
  else if( _protocol == EDP )
  {
    //unsigned char streamArrayCnt = 2;//sizeof(streamArray) / sizeof(streamArray[0]);
    FORMAT_TYPE type = FORMAT_TYPE3;

    EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};
    
    short body_len = 0;
    
    if(type < 1 && type > 5)
      return false;
    
    //DEBUG.printf("Tips:	OneNet_SendData-EDP_TYPE%d\r\n", type);
    
    if(type != kTypeBin)
    {
  //---------------------------------------------步骤一：测量数据流长度---------------------------------------------
      body_len = DSTREAM_GetDataStream_Body_Measure(type, streamArray, streamArrayCnt, 0);
      
      if(body_len)
      {
  //---------------------------------------------步骤二：填写协议头-------------------------------------------------
        if(EDP_PacketSaveData(_devid, body_len, NULL, (SaveDataType)type, &edpPacket) == 0)
        {
  //---------------------------------------------步骤三：组包-------------------------------------------------------
          body_len = DSTREAM_GetDataStream_Body(type, streamArray, streamArrayCnt, edpPacket._data, edpPacket._size, edpPacket._len);
          
          if(body_len)
          {
            edpPacket._len += body_len;
  //---------------------------------------------步骤四：发送数据---------------------------------------------------
            Send(edpPacket._data, edpPacket._len);
            //DEBUG.printf("Send %d Bytes\r\n", edpPacket._len);
          }
          else
            DEBUG.printf("WARN:	DSTREAM_GetDataStream_Body Failed\r\n");
          
  //---------------------------------------------步骤五：删包-------------------------------------------------------
          EDP_DeleteBuffer(&edpPacket);
          return true;
        }
        else
          DEBUG.printf("WARN:	EDP_NewBuffer Failed\r\n");
      }
      else
        DEBUG.printf("WARN:	DataStream Len Zero\r\n");
    }
    else
    {
      //OneNet_SendData_Picture(devid, Array, sizeof(Array));
    }
  }
  else
  {
    
  }
  return false;
}

//==========================================================
//	函数名称：	OneNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNET::RevPro(unsigned char *cmd,char* order, int* value)
{
	
	EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};	//协议包
	
	char *cmdid_devid = NULL;
	unsigned short cmdid_len = 0;
	char *req = NULL;
	unsigned int req_len = 0;
	unsigned char type = 0;
	
	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	type = EDP_UnPacketRecv(cmd);
	switch(type)										//判断是pushdata还是命令下发
	{
		case CMDREQ:									//解命令包
			
			result = EDP_UnPacketCmd(cmd, &cmdid_devid, &cmdid_len, &req, &req_len);
			
			if(result == 0)								//解包成功，则进行命令回复的组包
			{
				EDP_PacketCmdResp(cmdid_devid, cmdid_len, req, req_len, &edpPacket);
				//DEBUG.printf("cmdid: %s, req: %s, req_len: %d\r\n", cmdid_devid, req, req_len);
			}
			
		break;
			
		case SAVEACK:
			
			if(cmd[3] == MSG_ID_HIGH && cmd[4] == MSG_ID_LOW)
			{
				DEBUG.printf("Tips:	Send %s\r\n", cmd[5] ? "Err" : "Ok");
			}
			else
				DEBUG.printf("Tips:	Message ID Err\r\n");
			
		break;
			
		default:
			result = -1;
		break;
	}
	
	//ESP8266_Clear();									//清空缓存
	(*_esp8266).Clear();

	if(result == -1)
		return;
	
	dataPtr = strchr(req, ':');							//搜索':'
	
	if(dataPtr != NULL)									//如果找到了
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
		{
			numBuf[num++] = *dataPtr++;
		}
		numBuf[num] = 0;
		
		num = atoi((const char *)numBuf);				//转为数值形式
		
    uint8_t pos = strchr(req, ':') - req;
    strncat(order,req,pos);
    *value = num;
	}
	
	if(type == CMDREQ && result == 0)						//如果是命令包 且 解包成功
	{
		EDP_FreeBuffer(cmdid_devid);						//释放内存
		EDP_FreeBuffer(req);
															//回复命令
		Send(edpPacket._data, edpPacket._len);	//上传平台
		EDP_DeleteBuffer(&edpPacket);						//删包
	}

}
