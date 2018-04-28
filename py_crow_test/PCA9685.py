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




init_pca9685 (0x40)
init_pca9685 (0x42)

while True :


	# OFF
	#bus.write_byte_data (0x40, 0x06, 0x00)
	#bus.write_byte_data (0x40, 0x07, 0x00)


	# ALL
	bus.write_byte_data (0x40, 0xFA, 0x00)
	bus.write_byte_data (0x40, 0xFB, 0x00)

	bus.write_byte_data (0x42, 0xFA, 0x00)
	bus.write_byte_data (0x42, 0xFB, 0x00)



	
	time.sleep(0.05)
	

	for i in range (0, 4000, 80):


		bus.write_byte_data (0x40, 0xFA, 0x00)
		bus.write_byte_data (0x40, 0xFB, 0x00)

		bus.write_byte_data (0x42, 0xFA, 0x00)
		bus.write_byte_data (0x42, 0xFB, 0x00)


		bus.write_byte_data (0x40, 0xFC, ((4000-i)%4000) & 0xff)
		bus.write_byte_data (0x40, 0xFD, ((4000-i)%4000) / 0xff)	

		bus.write_byte_data (0x42, 0xFC, ((4000-i)%4000) & 0xff)
		bus.write_byte_data (0x42, 0xFD, ((4000-i)%4000) / 0xff)	
		
		
		# ON
		#bus.write_byte_data (0x40, 0x08, i & 0xff)
		#bus.write_byte_data (0x40, 0x09, i / 0xff)	


		#bus.write_byte_data (0x40, 0x0C, ((4000-i)%4000) & 0xff)
		#bus.write_byte_data (0x40, 0x0D, ((4000-i)%4000) / 0xff)	




		
		time.sleep(0.01)
