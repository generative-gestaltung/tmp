#include "patch.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

static Clock clock1 = {
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


void Patch_updateInpSEQ1 (StateInp* stateInp) {
	//memcpy (&_stateInp, state, sizeof(StateInp));
	int i;
	for (i=0; i<16; i++) {
		//sequence[i] = stateInp->encoders[i];
	}
}



void Patch_updateSEQ1 (State* state) {

	int len = gui_params[6];
	clock1.ds = FREQ_TABLE[127-gui_params[2]];

	if (len==0) len=1;
	updateClock (&clock1, 0.01);
	if (clock1.v ==1 && clock1.lastV ==0)
		step = (step+1) % len;

	state->cvOut[0] = PITCH_TABLE[sequence[step]]; //sequence[step] * 600;
	state->trOut[0] = (int)clock1.v;

	state->cvOut[1] = PITCH_TABLE[(sequence[step] + gui_params[11])%128]; //sequence[step] * 600;
	state->trOut[1] = (int)clock1.v;

	state->cvOut[2] = PITCH_TABLE[(sequence[step] + gui_params[11]*2)%128]; //sequence[step] * 600;
	state->trOut[2] = (int)clock1.v;

	state->cvOut[3] = PITCH_TABLE[(sequence[step]*2)%128]; //sequence[step] * 600;
	state->trOut[3] = (int)clock1.v;

	//printf("%d\n", state->trOut[0]);
	state->encLeds = (1<<step);
}

