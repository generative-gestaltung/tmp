#ifndef PATCH_H
#define PATCH_H

#include <inttypes.h>


#define WAVE_SIN 0
#define WAVE_SAW 1
#define WAVE_RAMP 2
#define WAVE_RECT 3

typedef struct State {
	uint8_t trOut[4];
	uint8_t trIn[4];
	float cvOut[4];
	uint16_t encLeds;
} State;

typedef struct StateInp {
	uint8_t trIn[4];
	int buttons[8];
	float encoders[16];
} StateInp;


State state0;

void Patch_update (State* state);
void Patch_updateInput (StateInp* state);

#endif
