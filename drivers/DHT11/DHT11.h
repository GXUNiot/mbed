#ifndef _DHT11_H
#define _DHT11_H

#include "mbed.h"
 
#define DHTLIB_OK                0
#define DHTLIB_ERROR_CHECKSUM   -1
#define DHTLIB_ERROR_TIMEOUT    -2
 
/** DHT11 温湿度传感器
 * 
 * Example:
 * @code
 *	#include "CNiot.h"
 *
 *	//使用 PC_13 脚连接到LED
 *	LedBlink blink(PC_13);
 *
 *	Serial PC(SERIAL_TX,SERIAL_RX);
 *
 *	DHT11 dht11(PB_9);
 *
 *	int main() {
 *			PC.baud(9600);
 *			while(1) {
 *					if( dht11.read() == DHTLIB_OK )
 *						PC.printf("T: %d, H: %d\r\n", dht11.getCelsius(), dht11.getHumidity());
 *					wait(1);
 *			}
 *	}
 *
 * @endcode
 */
class DHT11
{
public:
    /** Construct the sensor object.
     *
     * @param pin PinName for the sensor pin.
     */
    DHT11(PinName const &p);
    
    /** Update the humidity and temp from the sensor.
     *
     * @returns
     *   0 on success, otherwise error.
     */
    int read();
    
    /** Get the temp(f) from the saved object.
     *
     * @returns
     *   Fahrenheit float
     */
    float getFahrenheit();
    
    /** Get the temp(c) from the saved object.
     *
     * @returns
     *   Celsius int
     */
    int getCelsius();
    
    /** Get the humidity from the saved object.
     *
     * @returns
     *   Humidity percent int
     */
    int getHumidity();
 
private:
    /// percentage of humidity
    int _humidity;
    /// celsius
    int _temperature;
    /// pin to read the sensor info on
    DigitalInOut _pin;
    /// times startup (must settle for at least a second)
    Timer _timer;
};
 


#endif


