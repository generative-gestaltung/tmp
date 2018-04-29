import smbus
import time
import math
import random

bus = smbus.SMBus(1)


def init_pcf8575 (addr, openDrain=False):
	bus.write_byte_data (addr, 0x01, 0x04)
	bus.write_byte_data (addr, 0x00, 0x01)
	time.sleep(0.005)

	mode = bus.read_byte_data (addr, 0x00)
	bus.write_byte_data (addr, 0x00, 0x10)
	bus.write_byte_data (addr, 0xFE, 0x65)

	mode2 = 0
	if openDrain:
		mode2 = (1<<2)

	bus.write_byte_data (addr, 0x00, mode)
	bus.write_byte_data (addr, 0x01, mode2)
	time.sleep(0.005)

	bus.write_byte_data (addr, 0x00, mode | 0x80)



init_pcf8575 (0x20)



def set (addr, pin, v) :


	on = 0x06 + pin*2
	off = 0x08 + pin*2

	bus.write_byte_data (addr, on, v & 0xff)
	bus.write_byte_data (addr, on+1, v / 0xff)

	bus.write_byte_data (addr, off, 0)
	bus.write_byte_data (addr, off+1, 0)

