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
init_pca9685 (0x41)
init_pca9685 (0x42)
init_pca9685 (0x43)



def allOff (addr) :
	bus.write_byte_data (addr, 0xFA, 0x00)
	bus.write_byte_data (addr, 0xFB, 0x00)

	bus.write_byte_data (addr, 0xFC, 0xff)
	bus.write_byte_data (addr, 0xFD, 0xff)

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


cnt = 0
p2 = 0
while True :



	time.sleep(0.05)


	allOff (0x40)
	allOff (0x41)
	allOff (0x42)
	allOff (0x43)



	N = int(math.floor(random.random()*4))

	for i in range(0,N):
		p = int(math.floor(random.random()*8))
		#set (0x40, cnt*4, 0)
		set (0x40, p*4+2, 2000)
		set (0x40, p*4, 1000)

	set (0x41, p2*4+2, 2000)
	set (0x41, p2*4, 1000)

	p2 = (p2 + 1 )%8
	#set (0x40, 4, 2000)
	#set (0x40, 8, 2000)
	#set (0x40, 12, 2000)

	#set (0x40, 16, 2000)

	#set (0x41, 0, 2000)
	#set (0x41, 4, 2000)
	#set (0x41, 8, 2000)
	#set (0x41, 12, 2000)

	time.sleep(0.2)
