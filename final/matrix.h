#ifndef MATRIX_H
#define MATRIX_H

#include <inttypes.h>
#include <bcm2835.h>

#define PIN_X0 RPI_V2_GPIO_P1_11 // 11
#define PIN_X1 RPI_V2_GPIO_P1_12
#define PIN_X2 RPI_V2_GPIO_P1_13
#define PIN_X3 RPI_V2_GPIO_P1_15
#define PIN_X4 RPI_V2_GPIO_P1_16
#define PIN_X5 RPI_V2_GPIO_P1_18
#define PIN_X6 RPI_V2_GPIO_P1_22
#define PIN_X7 RPI_V2_GPIO_P1_29

#define PIN_Y0 RPI_V2_GPIO_P1_31
#define PIN_Y1 RPI_V2_GPIO_P1_32
#define PIN_Y2 RPI_V2_GPIO_P1_33
#define PIN_Y3 RPI_V2_GPIO_P1_35

#define PIN_TRIGGER_OUT0 RPI_V2_GPIO_P1_07
#define PIN_TRIGGER_OUT1 RPI_V2_GPIO_P1_08
#define PIN_TRIGGER_OUT2 RPI_V2_GPIO_P1_10
#define PIN_TRIGGER_OUT3 RPI_V2_GPIO_P1_26

#define PIN_TRIGGER_IN0 RPI_V2_GPIO_P1_36
#define PIN_TRIGGER_IN1 RPI_V2_GPIO_P1_37
#define PIN_TRIGGER_IN2 RPI_V2_GPIO_P1_38
#define PIN_TRIGGER_IN3 RPI_V2_GPIO_P1_40


typedef struct Enc {
	int min;
	int max;
	uint8_t last;
	int v;
} Enc;


void updateEnc (Enc* enc, uint8_t v);
Enc encoders[16];

void Matrix_init();
void Matrix_update();
void triggerOut (int ch, int v);

#endif
