#include "matrix.h"
#include "bcm2835.h"

int trigger_pins[4] = {
	RPI_V2_GPIO_P1_07,
	RPI_V2_GPIO_P1_08,
	RPI_V2_GPIO_P1_10,
	RPI_V2_GPIO_P1_26
};



int ENC_STATES[16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
int trOut[4] = {0,0,0,0};

void updateEnc (Enc* enc, uint8_t v) {
	enc->last <<= 2;
	enc->last |= v;
	enc->v -= ENC_STATES[enc->last&0x0f];
	if (enc->v < enc->min) enc->v = enc->min;
	if (enc->v > enc->max) enc->v = enc->max;
}

void triggerOut (int ch, int v) {

	if (trOut[ch] != v) {
		trOut[ch] = v;
		if (v==0) {
			bcm2835_gpio_write (trigger_pins[ch], LOW);
		}
		else {
			bcm2835_gpio_write (trigger_pins[ch], HIGH);
		}
	}
};

void Matrix_init() {

	int i;
	for (i=0; i<16; i++) {
		memset (encoders+i, 0, sizeof(Enc));
		encoders[i].min = 0;
		encoders[i].max = 255;
	}

	bcm2835_gpio_fsel (PIN_X0, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel (PIN_X1, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel (PIN_X2, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel (PIN_X3, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel (PIN_X4, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel (PIN_X5, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel (PIN_X6, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel (PIN_X7, BCM2835_GPIO_FSEL_INPT);

	bcm2835_gpio_fsel (PIN_Y0, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel (PIN_Y1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel (PIN_Y2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel (PIN_Y3, BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_write (PIN_Y0, LOW);
	bcm2835_gpio_write (PIN_Y1, LOW);
	bcm2835_gpio_write (PIN_Y2, LOW);
	bcm2835_gpio_write (PIN_Y3, LOW);



	bcm2835_gpio_fsel (PIN_TRIGGER_OUT0, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel (PIN_TRIGGER_OUT1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel (PIN_TRIGGER_OUT2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel (PIN_TRIGGER_OUT3, BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_fsel (PIN_TRIGGER_IN0, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel (PIN_TRIGGER_IN1, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel (PIN_TRIGGER_IN2, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel (PIN_TRIGGER_IN3, BCM2835_GPIO_FSEL_INPT);
}

uint8_t row = 0;
uint8_t x[8];
int cnt = 0;

void Matrix_update() {

	if (row==0) {
		bcm2835_gpio_write (PIN_Y0, HIGH);
		bcm2835_gpio_write (PIN_Y1, LOW);
		bcm2835_gpio_write (PIN_Y2, LOW);
		bcm2835_gpio_write (PIN_Y3, LOW);

		//bcm2835_gpio_fsel (PIN_Y0, BCM2835_GPIO_FSEL_OUTP);
		//bcm2835_gpio_fsel (PIN_Y1, BCM2835_GPIO_FSEL_INPT);
		//bcm2835_gpio_fsel (PIN_Y2, BCM2835_GPIO_FSEL_INPT);
		//bcm2835_gpio_fsel (PIN_Y3, BCM2835_GPIO_FSEL_INPT);
	}

	if (row==1) {
		bcm2835_gpio_write (PIN_Y0, LOW);
		bcm2835_gpio_write (PIN_Y1, HIGH);
		bcm2835_gpio_write (PIN_Y2, LOW);
		bcm2835_gpio_write (PIN_Y3, LOW);
	}

	if (row==2) {
		bcm2835_gpio_write (PIN_Y0, LOW);
		bcm2835_gpio_write (PIN_Y1, LOW);
		bcm2835_gpio_write (PIN_Y2, HIGH);
		bcm2835_gpio_write (PIN_Y3, LOW);
	}

	if (row==3) {
		bcm2835_gpio_write (PIN_Y0, LOW);
		bcm2835_gpio_write (PIN_Y1, LOW);
		bcm2835_gpio_write (PIN_Y2, LOW);
		bcm2835_gpio_write (PIN_Y3, HIGH);
	}

	usleep(1000);
	x[0] = bcm2835_gpio_lev (PIN_X0);
	x[1] = bcm2835_gpio_lev (PIN_X1);
	x[2] = bcm2835_gpio_lev (PIN_X2);
	x[3] = bcm2835_gpio_lev (PIN_X3);
	x[4] = bcm2835_gpio_lev (PIN_X4);
	x[5] = bcm2835_gpio_lev (PIN_X5);
	x[6] = bcm2835_gpio_lev (PIN_X6);
	x[7] = bcm2835_gpio_lev (PIN_X7);



	updateEnc (&encoders[0+row], x[1]<<1 | x[0]);
	updateEnc (&encoders[4+row], x[3]<<1 | x[2]);
	updateEnc (&encoders[8+row], x[5]<<1 | x[4]);
	updateEnc (&encoders[12+row], x[7]<<1 | x[6]);

	row = (row+1) % 4;

/*
	cnt = (cnt+1)%256;
	if (!cnt) {
		printf("%d %d %d %d \n", encoders[0].v, encoders[1].v, encoders[2].v, encoders[3].v);
		printf("%d %d %d %d \n", encoders[4].v, encoders[5].v, encoders[6].v, encoders[7].v);
		printf("%d %d %d %d \n", encoders[8].v, encoders[9].v, encoders[10].v, encoders[11].v);
		printf("%d %d %d %d \n", encoders[12].v, encoders[13].v, encoders[14].v, encoders[15].v);
		printf("\n");
	}
*/
}
