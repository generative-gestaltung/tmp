import RPi.GPIO as GPIO
import time
import smbus



I2C_BUS = smbus.SMBus(1)


print GPIO.VERSION
print I2C_BUS

#print I2C_BUS.read_byte_data (0x20, 1)


PIN_X0 = 11
PIN_X1 = 12
PIN_X2 = 13
PIN_X3 = 15
PIN_X4 = 16
PIN_X5 = 18
PIN_X6 = 22
PIN_X7 = 29

PIN_Y0 = 31
PIN_Y1 = 32
PIN_Y2 = 33
PIN_Y3 = 35

PIN_TRIGGER_OUT0 = 7
PIN_TRIGGER_OUT1 = 8
PIN_TRIGGER_OUT2 = 10
PIN_TRIGGER_OUT3 = 26

PIN_TRIGGER_IN0 = 36
PIN_TRIGGER_IN1 = 37
PIN_TRIGGER_IN2 = 38
PIN_TRIGGER_IN3 = 40


GPIO.setmode (GPIO.BOARD)



GPIO.setup (PIN_Y0, GPIO.OUT)
GPIO.setup (PIN_Y1, GPIO.OUT)
GPIO.setup (PIN_Y2, GPIO.OUT)
GPIO.setup (PIN_Y3, GPIO.OUT)

GPIO.setup (PIN_X0, GPIO.IN)
GPIO.setup (PIN_X1, GPIO.IN)
GPIO.setup (PIN_X2, GPIO.IN)
GPIO.setup (PIN_X3, GPIO.IN)
GPIO.setup (PIN_X4, GPIO.IN)
GPIO.setup (PIN_X5, GPIO.IN)
GPIO.setup (PIN_X6, GPIO.IN)
GPIO.setup (PIN_X7, GPIO.IN)



GPIO.setup (PIN_TRIGGER_OUT0, GPIO.OUT)
GPIO.setup (PIN_TRIGGER_OUT1, GPIO.OUT)
GPIO.setup (PIN_TRIGGER_OUT2, GPIO.OUT)
GPIO.setup (PIN_TRIGGER_OUT3, GPIO.OUT)

GPIO.setup (PIN_TRIGGER_IN0, GPIO.IN)
GPIO.setup (PIN_TRIGGER_IN1, GPIO.IN)
GPIO.setup (PIN_TRIGGER_IN2, GPIO.IN)
GPIO.setup (PIN_TRIGGER_IN3, GPIO.IN)



cnt = 0
PERIOD = 500

ENC_STATES = [0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0]

class Enc:

	def __init__(self):
		self.last = 0
		self.v = 0

	def update (self, v):
		self.last <<= 2
		self.last |= v
		self.v += ENC_STATES[self.last&0x0f]		
encoders = []
for i in range(0,16):
	encoders.append (Enc())

	

while True:


	time.sleep(0.001)

	row = cnt % 4
	GPIO.output (PIN_Y0, (row==0))
	GPIO.output (PIN_Y1, (row==1))
	GPIO.output (PIN_Y2, (row==2))
	GPIO.output (PIN_Y3, (row==3))


	x0 = GPIO.input (PIN_X0)
	x1 = GPIO.input (PIN_X1)
	x2 = GPIO.input (PIN_X2)
	x3 = GPIO.input (PIN_X3)
	x4 = GPIO.input (PIN_X4)
	x5 = GPIO.input (PIN_X5)
	x6 = GPIO.input (PIN_X6)
	x7 = GPIO.input (PIN_X7)

	encoders [row*4+0].update (x1<<1 | x0)
	encoders [row*4+1].update (x3<<1 | x2)
	encoders [row*4+2].update (x5<<1 | x4)
	encoders [row*4+3].update (x7<<1 | x6)




	if cnt==0:
		GPIO.output (PIN_TRIGGER_OUT0, 0)
		GPIO.output (PIN_TRIGGER_OUT1, 0)
		GPIO.output (PIN_TRIGGER_OUT2, 0)
		GPIO.output (PIN_TRIGGER_OUT3, 0)


	if cnt==PERIOD / 2:
		GPIO.output (PIN_TRIGGER_OUT0, 1)
		GPIO.output (PIN_TRIGGER_OUT1, 1)
		GPIO.output (PIN_TRIGGER_OUT2, 1)
		GPIO.output (PIN_TRIGGER_OUT3, 1)

	
	if cnt==0:		
		print encoders[0].v

	if cnt==10000:
		print GPIO.input (PIN_TRIGGER_IN0)
		print GPIO.input (PIN_TRIGGER_IN1)
		print GPIO.input (PIN_TRIGGER_IN2)
		print GPIO.input (PIN_TRIGGER_IN3)
		print ""
	
	cnt = (cnt+1)%PERIOD
