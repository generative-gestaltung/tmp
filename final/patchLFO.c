#include "patch.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

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
	.ds = 0x7fff
};

void Patch_updateInpLFO (StateInp* stateInp) {

	//memcpy (&_stateInp, state, sizeof(StateInp));
	lfo0.f = stateInp->encoders[0] / 255.;
	lfo0.A = stateInp->encoders[1] / 255.;
	lfo0.w = (int)(stateInp->encoders[2] / 64);
	rand0.ds = stateInp->encoders[4]+1;
	rand0.A = stateInp->encoders[5] / 255.;

	clock0.ds = 0xffff - stateInp->encoders[8] * 0xff;
}


void Patch_updateLFO (State* state) {

	updateLfo (&lfo0, 0.01);
	updateRand (&rand0, 0.01);
	updateClock (&clock0, 0.01);

	state->cvOut[0] = (int)(lfo0.v * 65335);
	state->cvOut[1] = (int)(rand0.v * 65335);
	state->cvOut[2] = fmod (state->cvOut[2] + 0.3, 4096);
	state->cvOut[3] = fmod (state->cvOut[3] + 0.04, 4096);

	state->trOut[0] = clock0.v;
}
