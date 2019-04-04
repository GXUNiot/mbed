
#include "PZEM004T.h"

PZEM004T::PZEM004T(PinName tx,PinName rx) : _serial(tx, rx)
{
	_serial.baud(9600);
	_cnt = 0;
	_irqFlag = false;
	_serial.attach(callback(this,&PZEM004T::serial_IRQHandler));
}

int PZEM004T::readable(void)
{
	for(int i=0; i<sizeof(_buf); i++)
	{
		switch(_buf[i])
		{
		case 0xA0:
					_cnt = i;
					return 1;
		case 0xA1:
					_cnt = i;
					return 2;
		case 0xA2:
					_cnt = i;
					return 3;
		case 0xA3:
					_cnt = i;
					return 4;
		case 0xA4:
					_cnt = i;
					return 5;
		case 0xA5:
					_cnt = i;
					return 6;
		default: ;
		
		}
	}
	return PZEM004T_DATA_ERROR;
}

bool PZEM004T::check(void)
{
	unsigned char sum = 0;
	for(int i=0; i<6; i++)
		sum += _buf[i];
	if(sum == _buf[6])
		return true;
	else
		return false;
}

void PZEM004T::clear(void)
{
	memset(_buf,0,sizeof(_buf));
	_cnt = 0;
}

int PZEM004T::Test(void)
{
	unsigned char timeOut = 200;
	_serial.putc(0xB1);
	_serial.putc(0xC0);
	_serial.putc(0xA8);
	_serial.putc(0x01);
	_serial.putc(0x01);
	_serial.putc(0x00);
	_serial.putc(0x1B);
	
	while(!_irqFlag && timeOut--)
		wait_ms(5);
	_irqFlag = false;
	
	//printf("%x %x %x %x %x %x %x \r\n",_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
	
	if( _buf[0] != 0xA1)
		return PZEM004T_DATA_ERROR;
	if( !check() )
		return PZEM004T_CHECK_ERROR;
	
	return ((int)_buf[2]*100)+((int)_buf[3]);
	
}

int PZEM004T::getVoltage(void)
{
	unsigned char timeOut = 200;
	_serial.putc(0xB0);
	_serial.putc(0xC0);
	_serial.putc(0xA8);
	_serial.putc(0x01);
	_serial.putc(0x01);
	_serial.putc(0x00);
	_serial.putc(0x1A);
	
	while(!_irqFlag && timeOut--)
		wait_ms(5);
	_irqFlag = false;
	
	
	if( _buf[0] != 0xA0)
		return PZEM004T_DATA_ERROR;
	if( !check() )
		return PZEM004T_CHECK_ERROR;
	
	return (((int)(_buf[1]<<8)|_buf[2])*10)+((int)_buf[3]);
}
	
int PZEM004T::getCurrent(void)
{
	unsigned char timeOut = 200;
	_serial.putc(0xB1);
	_serial.putc(0xC0);
	_serial.putc(0xA8);
	_serial.putc(0x01);
	_serial.putc(0x01);
	_serial.putc(0x00);
	_serial.putc(0x1B);
	
	while(!_irqFlag && timeOut--)
		wait_ms(5);
	_irqFlag = false;
	
	
	if( _buf[0] != 0xA1)
		return PZEM004T_DATA_ERROR;
	if( !check() )
		return PZEM004T_CHECK_ERROR;
	
	return (((int)(_buf[1]<<8)|(_buf[2]))*10)+((int)_buf[3]);
}

int PZEM004T::getActivepower(void)
{
	unsigned char timeOut = 200;
	_serial.putc(0xB2);
	_serial.putc(0xC0);
	_serial.putc(0xA8);
	_serial.putc(0x01);
	_serial.putc(0x01);
	_serial.putc(0x00);
	_serial.putc(0x1C);
	
	while(!_irqFlag && timeOut--)
		wait_ms(5);
	_irqFlag = false;
	
	
	if( _buf[0] != 0xA2)
		return PZEM004T_DATA_ERROR;
	if( !check() )
		return PZEM004T_CHECK_ERROR;
	
	return ((int)(_buf[1]<<8)|_buf[2]);
}
	
int PZEM004T::getElectricity(void)
{
	unsigned char timeOut = 200;
	_serial.putc(0xB3);
	_serial.putc(0xC0);
	_serial.putc(0xA8);
	_serial.putc(0x01);
	_serial.putc(0x01);
	_serial.putc(0x00);
	_serial.putc(0x1D);
	
	while(!_irqFlag && timeOut--)
		wait_ms(5);
	_irqFlag = false;
	
	
	if( _buf[0] != 0xA3)
		return PZEM004T_DATA_ERROR;
	if( !check() )
		return PZEM004T_CHECK_ERROR;
	
	return ((int)((_buf[1]<<16)|(_buf[2]<<8)|_buf[3]));
}

void PZEM004T::serial_IRQHandler()
{
	if(_serial.readable())
	{
		_buf[_cnt] = _serial.getc();
		if(_cnt == 0 && (_buf[0]&0xA0)!=0xA0)
			return;
		
		_cnt++;
		if(_cnt>=7)
		{
			_cnt = 0;
			_irqFlag = true;
		}
			
		
	}
}
