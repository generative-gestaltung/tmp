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
	.lastV = 0.0,
	.ds = 0x1fff,
	.cnt = 0
};

int step = 0;
	int sequence[16] = {0, 3, 7, 12,
                     0, 3, 7, 12,
		     5, 10, 17, 22,
                     5, 10, 17, 22};


void Patch_updateInpSEQ (StateInp* stateInp) {
	//memcpy (&_stateInp, state, sizeof(StateInp));
	int i;
	for (i=0; i<16; i++) {
		//sequence[i] = stateInp->encoders[i];
	}
}



void Patch_updateSEQ (State* state) {

	int len = gui_params[8];
	clock1.ds = FREQ_TABLE[127-gui_params[9]];

	if (len==0) len=1;


	updateClock (&clock1, 0.01);
	if (clock1.v ==1 && clock1.lastV ==0)
		step = (step+1) % len;

	state->cvOut[0] = PITCH_TABLE[sequence[step]]; //sequence[step] * 600;
	state->trOut[0] = (int)clock1.v;

	//printf("%d\n", state->trOut[0]);
	state->encLeds = (1<<step);
}
