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

char PATCH = 'A';
char NEW_PATCH = 'A';

void *fileFunc (void *x_void_ptr) {

	while(1) {

		FILE* f = fopen("/home/maxg/dev/tmp/final/xxx", "rb");
		int i;
    		size_t lSize;
    		fseek (f , 0 , SEEK_END);
    		lSize = ftell (f);
    		rewind (f);

		uint8_t buf[2];
    		for (i=0; i<lSize/2; i++) {
			fread (&buf, 1, 2, f);
			//printf("%d %d\n", buf[0], buf[1]);
			gui_params[buf[0]] = buf[1];
		}
   		close(f);

		delay(500);

		f = fopen("/home/maxg/dev/tmp/final/yyy", "r");
		fread (&buf, 1, 1, f);

		NEW_PATCH = buf[0];
		PATCH = buf[0];

		//printf("%c %c\n", buf[0], NEW_PATCH);
		close(f);
		delay(500);
	}
}


void *mainFunc (void *x) {

	int i=0;
	int cnt = 0;
	Patch_init (&state0);

	while(1) {

		for (i=0; i<16; i++) {
			stateInp1.encoders[i] = encoders[i].v;
		}

		if (PATCH=='A') {
			Patch_updateInpLFO (&stateInp1);
		}
		else if (PATCH=='B') {
			Patch_updateInpSEQ0 (&stateInp1);
		}
		else if (PATCH=='C') {
			Patch_updateInpSEQ1 (&stateInp1);
		}

		memcpy (&state1, &state0, sizeof(State));

		if (PATCH=='A') {
			Patch_updateLFO (&state0);
		}
		else if (PATCH=='B') {
			Patch_updateSEQ0 (&state0);
		}
		else if (PATCH=='C') {
			Patch_updateSEQ1 (&state0);
		}


		DAC8564_Write (CHANNEL_A, state0.cvOut[0]);
		DAC8564_Write (CHANNEL_B, state0.cvOut[1]);
		DAC8564_Write (CHANNEL_C, state0.cvOut[2]);
		DAC8564_Write (CHANNEL_D, state0.cvOut[3]);

		triggerOut (0, state0.trOut[0]);
		triggerOut (1, state0.trOut[1]);
		triggerOut (2, state0.trOut[2]);
		triggerOut (3, state0.trOut[3]);

		cnt = (cnt+1) % 10000;
	}
}


void *readMatrix (void *x_void_ptr) {

	int *x_ptr = (int *)x_void_ptr;
	Matrix_init();

	while(1) {
		delay(1);
		Matrix_update();
	}
	return NULL;
}


void *writeLeds (void* x) {

	wiringPiSetup();
	int i;
	int cnt = 0;

	for (i=0; i<4; i++) {
		i2c_led_fd[i] = wiringPiI2CSetup (I2C_LED_PORTS[i]);
	}

	init_pca9685 (0, 0);
	init_pca9685 (1, 0);
	init_pca9685 (2, 0);
	init_pca9685 (3, 1);

	allOff (0);
	allOff (1);
	allOff (2);
	allOn (3);


	while(1) {
		int i;
		for (i=0; i<4; i++) {
			int tr = state1.trOut[i];
			if (tr != lastTrOut[i]) {
				lastTrOut[i] = tr;
				if (tr) {
					set (3, 16+2*i, 0xffff);
				}
				else {
					allOn (3);
					set (3, 16+2*i, 0xffff);
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

	int i;
	printf("START %c\n", PATCH);

	DAC8564_Init();

	int x = 0, y = 0, z = 0, w = 0;

	pthread_t matrix_thread, led_thread, main_thread, file_thread;
	//Patch_init (&state0);
	//Matrix_init();

	if(pthread_create(&main_thread, NULL, mainFunc, &x)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	if(pthread_create (&matrix_thread, NULL, readMatrix, &y)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	if(pthread_create (&led_thread, NULL, writeLeds, &z)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	if(pthread_create (&file_thread, NULL, fileFunc, &w)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}


	int cnt = 0;

	while(1) {

		delay(1);
#if 0
		for (i=0; i<16; i++) {
			stateInp1.encoders[i] = encoders[i].v;
		}

		if (PATCH=='s') {
			Patch_updateInpSEQ (&stateInp1);
		}
		else {
			Patch_updateInpLFO (&stateInp1);
		}

		memcpy (&state1, &state0, sizeof(State));

		if (PATCH=='s') {
			Patch_updateSEQ (&state0);

		}
		else {
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

		cnt = (cnt+1) % 10000;
#endif
	}

	printf("done\n");
	return 0;
}
