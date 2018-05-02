#ifndef PATCH_H
#define PATCH_H

#include <inttypes.h>


#define TWO_PI 6.283185307

#define WAVE_SIN 0
#define WAVE_SAW 1
#define WAVE_RAMP 2
#define WAVE_RECT 3

typedef struct Rand {
	float v;
	int cnt;
	int ds;
	float A;
} Rand;


typedef struct Clock {
	float v;
	int cnt;
	int ds;
	float lastV;
} Clock;


typedef struct Lfo {
	float p;
	float f;
	float A;
	float v;
	int w;
} Lfo;


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


StateInp _stateInp;
State state0;

int sign (float x);
void updateClock (Clock* clock, float dt);
void updateRand (Rand* rnd, float dt);
void updateLfo (Lfo* lfo, float dt);



void Patch_init (State* state);


void Patch_updateLFO (State* state);
void Patch_updateInputLFO (StateInp* state);

void Patch_updateSEQ (State* state);
void Patch_updateInputSEQ (StateInp* state);

#endif





