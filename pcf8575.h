 #ifndef MBED_PCF8575_H
#define MBED_PCF8575_H
 
/** Interface to the PCF8575 I2C 16 Bit IO expander */
class PCF8575 {
public:
    /** Create an instance of the PCF8575 connected to specfied I2C pins, with the specified address.
     *
     * @param sda The I2C data pin
     * @param scl The I2C clock pin
     * @param address The I2C address for this PCF8575
     */
    PCF8575(PinName sda, PinName scl, int address);
 
    /** Read the IO pin level
     *
     * @return The two bytes read
     */
    int read();
    
    /** Write to the IO pins
     * 
     * @param data The 16 bits to write to the IO port
     */
    void write(int data);
 
private:
    //I2C _i2c;
    int _address;
};
 
#endif
 
