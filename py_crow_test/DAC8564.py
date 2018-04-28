import time
import sys
import spidev
import math

PI = 3.1417

DISABLE_INT_REF = 0x012000
ENABLE_INT_REF  = 0x010000

WRITE_BUFFER_A  = 0x000000
WRITE_BUFFER_B  = 0x020000
WRITE_BUFFER_C  = 0x040000
WRITE_BUFFER_D  = 0x060000

WRITE_BUFFER_A_UPDATE  = 0x200000
WRITE_BUFFER_B_UPDATE  = 0x220000
WRITE_BUFFER_C_UPDATE  = 0x240000
WRITE_BUFFER_D_UPDATE  = 0x260000


UPDATE_ALL_CH   = 0x300000



spi = spidev.SpiDev()
spi.open (0,0)


cnt = 0

while True:
	
	data0 = int (math.floor (math.sin(cnt*0.04 + PI/4)*32000 + 32000))
	data1 = int (math.floor (math.sin(cnt*0.04 + PI/2)*32000 + 32000))
	data2 = int (math.floor (math.sin(cnt*0.04 + 3*PI/4)*32000 + 32000))
	data3 = int (math.floor (math.sin(cnt*0.04 + PI/2)*32000 + 32000))

	
	spi.xfer2 ([0x00, data0 >> 8, data0 & 0xff])
	spi.xfer2 ([0x02, data1 >> 8, data1 & 0xff])
	spi.xfer2 ([0x04, data2 >> 8, data2 & 0xff])
	spi.xfer2 ([0x26, data3 >> 8, data3 & 0xff])

	cnt += 1

	time.sleep(0.001)
