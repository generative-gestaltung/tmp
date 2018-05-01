#include "patch.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

StateInp _stateInp;

#define TWO_PI 6.283185307

int sequence[16] = {1,3,1,12,5,5,1,12,1,3,1,12,5,5,1,12};


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
	int cntSeq;
} Clock;


typedef struct Lfo {
	float p;
	float f;
	float A;
	float v;
	int w;
} Lfo;

Lfo lfo0 = {
	.p = 0.0,
	.f = 0.1,
	.A = 1.0,
	.v = 0.0,
	.w = WAVE_SIN
};

Rand rand0 = {
	.v = 0.0,
	.cnt = 0,
	.ds = 1,
	.A = 1.0
};

Clock clock0 = {
	.v = 0.0,
	.cnt = 0,
	.ds = 0x7fff,
	.lastV = 0.0,
	.cnt = 0,
	.cntSeq = 0
};

int sign (float x) {
	if (x<0) return -1;
	else return 1;
}



void updateClock (Clock* clock, float dt) {
	clock->cnt = (clock->cnt+1) % (clock->ds);
	if (clock->cnt == 0)
		clock->v = 1;
	if (clock->cnt == clock->ds/2)
		clock->v = 0;

	if (clock->v == 1.0 && clock->lastV == 0.0) {
		clock->cntSeq = (clock->cntSeq+1)%16;
	}
	clock->lastV = clock->v;
}

void updateRand (Rand* rnd, float dt) {
	rnd->cnt = (rnd->cnt+1) % (rnd->ds*8);
	if (!rnd->cnt)
		rnd->v = (rand() % 256) / 256.0 * rnd->A;
}

void updateLfo (Lfo* lfo, float dt) {
	lfo->p += (dt * lfo->f);

	if (lfo->w == WAVE_SIN)
		lfo->v = (sin(lfo->p)*0.5+0.5)*lfo->A;
	else if (lfo->w == WAVE_SAW)
		lfo->v = fmod(lfo->p, TWO_PI) / TWO_PI * lfo->A;
	else if (lfo->w == WAVE_RAMP)
		lfo->v = (1-fmod(lfo->p, TWO_PI) / TWO_PI) * lfo->A;
	else if (lfo->w == WAVE_RECT) {
		lfo->v = (sign(sin(lfo->p))*0.5+0.5)*lfo->A;
	}
}

void Patch_init (State* state) {
	memset (&_stateInp, 0, sizeof (StateInp));
	memset (state, 0, sizeof(State));

}


void Patch_updateInp (StateInp* state) {
	memcpy (&_stateInp, state, sizeof(StateInp));
	lfo0.f = _stateInp.encoders[0] / 255.;
	lfo0.A = _stateInp.encoders[1] / 255.;
	lfo0.w = (int)(_stateInp.encoders[2] / 64);
	rand0.ds = _stateInp.encoders[4]+1;
	rand0.A = _stateInp.encoders[5] / 255.;

	clock0.ds = 0x4fff - _stateInp.encoders[8] * 0xff;
}


void Patch_update (State* state) {

	updateLfo (&lfo0, 0.01);
	updateRand (&rand0, 0.01);
	updateClock (&clock0, 0.01);

	state->cvOut[0] = sequence[clock0.cntSeq] * 0x02ff;
	state->cvOut[1] = (int)(rand0.v * 65335);
	state->cvOut[2] = fmod (state->cvOut[2] + 0.3, 4096);
	state->cvOut[3] = fmod (state->cvOut[3] + 0.04, 4096);

	state->trOut[0] = clock0.v;
	state->encLeds = (1<<clock0.cntSeq);
}
