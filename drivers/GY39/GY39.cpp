
#include "GY39.h"


GY39::GY39(PinName tx, PinName rx) : _serial(tx,rx)
{
	_serial.baud(9600);
	_counter = 0;
	readflag = false;
	_dataType = Wait;
	_serial.attach(callback(this,&GY39::Serial_IRQHandler));
	
	
	_serial.putc(0XA5);
	_serial.putc(0X83);
	_serial.putc(0X28);

}

bool GY39::read(void)
{
	if(readflag)
	{
		readflag = false;
		return true;
	}
	else
	{
		return false;
	}
	
}

int GY39::getLux(void)
{
	return _Lux;
}

float GY39::getPressure(void)
{
	return _P;
}

float GY39::getTemp(void)
{
	return _Temp;
}

float GY39::getHum(void)
{
	return _Hum;
}
	
int GY39::getAlt(void)
{
	return _Alt;
}

void GY39::Serial_IRQHandler()
{
	unsigned char sum = 0;
	unsigned short int data[2] = {0};
	while(_serial.readable())
	{
		_buf[_counter] = _serial.getc();
		if(_counter == 0 && _buf[0] !=0X5A)
			return;
		_counter++;
		if(_buf[2] == 0x15 && _counter == 9)
		{
			for(int i=0; i<8; i++)
				sum += _buf[i];
			if(sum == _buf[8])
			{
				data[0] = (_buf[4]<<8)|_buf[5];
				data[1] = (_buf[6]<<8)|_buf[7];
				_Lux = ((((unsigned int)data[0])<<16)|data[1])/100;
			}
			_counter = 0;
			readflag = true;
		}	
		else if(_buf[2] == 0x45 && _counter == 15)
		{
			for(int i=0; i<14; i++)
				sum += _buf[i];
			if(sum == _buf[14])
			{

				_Temp = ((float)((_buf[4]<<8)|_buf[5])/100);
					
				data[0] = (_buf[6]<<8)|_buf[7];
				data[1] = (_buf[8]<<8)|_buf[9];
				_P = (float)((((unsigned int)data[0])<<16)|data[1]/100);
				
				_Hum = ((float)((_buf[10]<<8)|_buf[11])/100);
					
				_Alt = ((_buf[12]<<8)|_buf[13]);
				
			}
			_counter = 0;
			readflag = true;
		}
		else if( _counter >= 15)
		{
			memset(_buf, 0, sizeof(_buf));
			_counter = 0;
		}
	}	
}
