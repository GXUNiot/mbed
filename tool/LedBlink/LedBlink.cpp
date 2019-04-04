#include <LedBlink.h>

LedBlink::LedBlink(PinName pin,float time ):_pin(pin)
{
    LedBlink::_time = time;
    LedBlink::run();
}

void LedBlink::run(void)
{
    LedBlink::_tBlink.attach(callback(this,&LedBlink::_flash),LedBlink::_time);
}

void LedBlink::stop(void)
{
    LedBlink::_tBlink.detach();
}

void LedBlink::_flash(void)
{
    LedBlink::_pin = ! LedBlink::_pin;
}

LedBlink::operator uint8_t()
{
    return LedBlink::_pin.read();
}
