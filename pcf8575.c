#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <inttypes.h>


void _write (int port, int data) {
    char foo[2];
    foo[0]=data;
    foo[1]=data>>8;
    wiringPiI2CWrite (port, foo);
    //_i2c.write(_address, foo, 2);
}



int main {
    int addr = 0x21;
    wiringPiSetup () ;
    port0 = wiringPiI2CSetup (addr);

    _write (addr, 0xffff);

}

/*
int PCF8575::read() {
    char foo[2];
    //_i2c.read(_address, foo, 2);
    return (foo[1] << 8) | foo[0];
}
*/



