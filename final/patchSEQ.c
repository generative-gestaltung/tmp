#include "patch.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

Lfo lfo1 = {
	.p = 0.0,
	.f = 0.1,
	.A = 1.0,
	.v = 0.0,
	.w = WAVE_SIN
};

Rand rand1 = {
	.v = 0.0,
	.cnt = 0,
	.ds = 1,
	.A = 1.0
};

Clock clock1 = {
	.v = 0.0,
	.cnt = 0,
	.ds = 0x0fff,
	.lastV = 0.0,
	.cnt = 0,
	.cntSeq = 0
};

int sequence[16] = {1,2,3,6,1,2,3,1,1,2,3,4,1,2,3,1};

void Patch_updateInpSEQ (StateInp* stateInp) {
	//memcpy (&_stateInp, state, sizeof(StateInp));
	int i;
	for (i=0; i<16; i++) {
		sequence[i] = stateInp->encoders[i];
	}
}


void Patch_updateSEQ (State* state) {

	updateClock (&clock1, 0.01);
	state->cvOut[0] = sequence[clock1.cntSeq] * 0x00ff;

	state->trOut[0] = clock1.v;
	state->encLeds = (1<<clock1.cntSeq);
}
