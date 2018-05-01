#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <inttypes.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "dac.h"
#include "matrix.h"
#include "patch.h"

#define DEBUG 0


StateInp stateInp1;
State state1;

uint8_t I2C_LED_PORTS[4] = {
	0x40, 0x41, 0x42, 0x43
};

int lastTrOut[4] = {0,0,0,0};

uint8_t i2c_led_fd [4];
char PATCH = '0';


void *readMatrix (void *x_void_ptr) {

	int *x_ptr = (int *)x_void_ptr;
	int cnt = 0;
	Matrix_init();
	char lastPatch = '0';

	while(1) {
		//delay(1);
		Matrix_update();
		cnt = (cnt+1)%1024;
	}
	return NULL;
}


void *writeLeds (void* x) {

	int cnt = 0;

	while(1) {
		int i;
		for (i=0; i<4; i++) {
			int tr = state1.trOut[i];
			if (tr != lastTrOut[i]) {
				lastTrOut[i] = tr;
				if (tr) {
					set (3, 16+2*i, 0);
				}
				else {
					allOn (3);
					set (3, 16+2*i, 0x0fff);
				}
			}
		}

		for (int i=0; i<16; i++) {
			int row = i/8;
			if ((state1.encLeds>>i) & 0x01) {
				set (row, (i%8)*4+2, 2000);
				set (row, (i%8)*4, 1000);
			}
			else {
				set (row, (i%8)*4, 0);
				set (row, (i%8)*4+2, 0);
			}
		}
	}

	return NULL;
}



void init_pca9685 (int ch, int openDrain) {



	int x = 0, y = 0;

	pthread_t matrix_thread, led_thread;

	if(pthread_create(&matrix_thread, NULL, readMatrix, &x)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	if(pthread_create(&led_thread, NULL, writeLeds, &y)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	int fd = i2c_led_fd[ch];

	write_byte_data (fd, 0x01, 0x04);
	write_byte_data (fd, 0x00, 0x01);

	delay(5);
	uint8_t mode = wiringPiI2CReadReg8 (fd, 0x00);
	write_byte_data (fd, 0x00, 0x10);
	write_byte_data (fd, 0xFE, 0x65);

	uint8_t mode2 = (1<<2);
	if (openDrain)
		mode2 = 0;

	write_byte_data (fd, 0x00, mode);
	write_byte_data (fd, 0x01, mode2);
	delay(5);
	write_byte_data (fd, 0x00, mode | 0x80);

}


void write_byte_data (int addr, int reg, int dat) {
	int ret = wiringPiI2CWriteReg8 (addr, reg, dat);
#if DEBUG
	printf ("w %d\n", ret);
#endif

}

void allOff (int ch) {

	int addr = i2c_led_fd[ch];

	write_byte_data (addr, 0xFA, 0x00);
	write_byte_data (addr, 0xFB, 0x00);

	write_byte_data (addr, 0xFC, 0xff);
	write_byte_data (addr, 0xFD, 0xff);
}

void allOn (int ch) {

	int addr = i2c_led_fd[ch];

	write_byte_data (addr, 0xFA, 0x00);
	write_byte_data (addr, 0xFB, 0x00);

	write_byte_data (addr, 0xFC, 0xff);
	write_byte_data (addr, 0xFD, 0x0f);
}

void set (int ch, int pin, int v) {

	int addr = i2c_led_fd[ch];

	uint8_t on = 0x06 + pin*2;
	uint8_t off = 0x08 + pin*2;

	//write_byte_data (addr, on, v & 0xff);
	//write_byte_data (addr, on+1, v / 0xff);
	//write_byte_data (addr, off, 0);
	//write_byte_data (addr, off+1, 0);

	write_byte_data (addr, off, v & 0xff);
	write_byte_data (addr, off+1, v / 0xff);
	write_byte_data (addr, on, 0);
	write_byte_data (addr, on+1, 0);
}




int main (int argc, char** argv) {

	PATCH = argv[1][0];
	int i;
	printf("START %c\n", PATCH);
	DAC8564_Init();
	DAC8564_Write (0, 4000);

	wiringPiSetup();

	for (i=0; i<4; i++) {
		i2c_led_fd[i] = wiringPiI2CSetup (I2C_LED_PORTS[i]);

#if DEBUG
		printf("init %d\n", i2c_led_fd[i]);
#endif
	}


	Patch_init (&state0);

	init_pca9685 (0, 0);
	init_pca9685 (1, 0);
	init_pca9685 (2, 0);
	init_pca9685 (3, 1);

	printf("init done\n");
	allOff (0);
	allOff (1);
	allOff (2);
	allOn (3);

	uint16_t V = 0;
	int cnt = 0;

	clock_t t0, t1;
    	t0 = clock();
	long time = 0;


	while(1) {


   		t1 = clock();
		int dt = (t1-t0); //)/CLOCKS_PER_SEC;
		t0 = t1;
		time += dt;

		if (!cnt) {
			//printf("%d\n", time);
		}
		if (time > 100) {
			time -= 100;
			//printf("%d \n", time);
			for (i=0; i<16; i++) {
				stateInp1.encoders[i] = encoders[i].v;
			}

			if (PATCH=='s')
				Patch_updateInpSEQ (&stateInp1);
			else
				Patch_updateInpLFO (&stateInp1);
			memcpy (&state1, &state0, sizeof(State));

			if (PATCH=='s')
				Patch_updateSEQ (&state0);
			else
				Patch_updateLFO (&state0);
		}
		DAC8564_Write (CHANNEL_A, state0.cvOut[0]);
		DAC8564_Write (CHANNEL_B, state0.cvOut[1]);
		DAC8564_Write (CHANNEL_C, state0.cvOut[2]);
		DAC8564_Write (CHANNEL_D, state0.cvOut[3]);

		triggerOut (0, state0.trOut[0]);
		triggerOut (1, state0.trOut[1]);
		triggerOut (2, state0.trOut[2]);
		triggerOut (3, state0.trOut[3]);

		cnt = (cnt+1) % 1024;
		//delay(1);
	}



	printf("done\n");
	return 0;
}



/*
cnt = 0
p2 = 0
while True :



	time.sleep(0.05)


	allOn (0x40)
	allOn (0x41)
	allOn (0x42)
	allOn (0x43)

	'''


	N = int(math.floor(random.random()*4))

	for i in range(0,N):
		p = int(math.floor(random.random()*8))
		#set (0x40, cnt*4, 0)
		set (0x40, p*4+2, 2000)
		set (0x40, p*4, 1000)

	set (0x41, p2*4+2, 2000)
	set (0x41, p2*4, 1000)

	p2 = (p2 + 1 )%8

	set (0x43, 16+(p2%4)*2, 2000)
	set (0x43, 28, 4000)
	set (0x43, 30, 4000)
	#set (0x40, 4, 2000)
	#set (0x40, 8, 2000)
	#set (0x40, 12, 2000)

	#set (0x40, 16, 2000)

	#set (0x41, 0, 2000)
	#set (0x41, 4, 2000)
	#set (0x41, 8, 2000)
	#set (0x41, 12, 2000)

	time.sleep(0.2)
	'''

    while(1) {

        if(tmp==0) {
     			Write_DAC8532(0x30, Voltage_Convert(5.0,0.00+(float)i/10));    	//Write channel A buffer (0x30)
    			Write_DAC8532(0x34, Voltage_Convert(5.0,5.000-(float)i/10));    	//Write channel B buffer (0x34)		
    			i++;

			if(i==50) {
				i=0;
				 tmp=1;
			bsp_DelayUS(500);
     	}

        else if(tmp==1) {
			Write_DAC8532(0x30, Voltage_Convert(5.0,5.000-(float)i/10));    	//Write channel B buffer (0x30)	
			Write_DAC8532(0x34, Voltage_Convert(5.0,0.00+(float)i/10));    	//Write channel A buffer (0x34)

    			i++;
			if(i==50) {
			    i=0;
			    tmp=0;
			}
			bsp_DelayUS(500);
        }
    }

    bcm2835_spi_end();
    bcm2835_close();

    return 0;
}
*/
