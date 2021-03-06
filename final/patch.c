#include "patch.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

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
		clock->cntSeq++; // = (clock->cntSeq+1)%16;
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
	memset (gui_params, 0, sizeof(gui_params));
}
