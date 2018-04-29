import smbus
import time
import math
import random

bus = smbus.SMBus(1)


def init_pca9685 (addr):
	bus.write_byte_data (addr, 0x01, 0x04)
	bus.write_byte_data (addr, 0x00, 0x01)
	time.sleep(0.005)

	mode = bus.read_byte_data (addr, 0x00)
	bus.write_byte_data (addr, 0x00, 0x10)
	bus.write_byte_data (addr, 0xFE, 0x65)

	bus.write_byte_data (addr, 0x00, mode)
	time.sleep(0.005)

	bus.write_byte_data (addr, 0x00, mode | 0x80)



#init_pca9685 (0x20)
init_pca9685 (0x40)
#init_pca9685 (0x42)



def allOff (addr) :
	bus.write_byte_data (addr, 0xFA, 0x00)
	bus.write_byte_data (addr, 0xFB, 0x00)

	bus.write_byte_data (addr, 0xFC, 0x00)
	bus.write_byte_data (addr, 0xFD, 0x00)

def allOn (addr) :
	bus.write_byte_data (addr, 0xFA, 0x00)
	bus.write_byte_data (addr, 0xFB, 0x00)

	bus.write_byte_data (addr, 0xFC, 0xff)
	bus.write_byte_data (addr, 0xFD, 0x0f)
	

def set (addr, pin, v) :


	on = 0x06 + pin*2
	off = 0x08 + pin*2

	bus.write_byte_data (addr, on, v & 0xff)
	bus.write_byte_data (addr, on+1, v / 0xff)	

	bus.write_byte_data (addr, off, 0)
	bus.write_byte_data (addr, off+1, 0)	



while True :



	
	time.sleep(0.05)


	allOff (0x40)
	allOff (0x42)


	
		
