#include "patch.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define baseSpeed  0x0fff



static Clock clock0 = {
	.v = 0.0,
	.lastV = 0.0,
	.ds = baseSpeed,
	.cnt = 0
};


static Clock clock1 = {
	.v = 0.0,
	.lastV = 0.0,
	.ds = baseSpeed*2,
	.cnt = 0
};

static Clock clock2 = {
	.v = 0.0,
	.lastV = 0.0,
	.ds = baseSpeed*3,
	.cnt = 0
};

static Clock clock3 = {
	.v = 0.0,
	.lastV = 0.0,
	.ds = baseSpeed*4,
	.cnt = 0
};


int cnt0=0;
int cnt1=0;
int cnt2=0;
int cnt3=0;

int sequence0[4] = { 0, 3, 7, 12 };
int sequence1[4] = { 0, 3, 7, 12 };
int sequence2[4] = { 0, 3, 7, 12 };
int sequence3[4] = { 0, 3, 7, 12 };


void Patch_updateInpSEQ (StateInp* stateInp) {
	int i;
	for (i=0; i<4; i++) {
		sequence0[i] = stateInp->encoders[i] / 4;
		sequence1[i] = stateInp->encoders[i+4] / 4;
		sequence2[i] = stateInp->encoders[i+8] / 4;
		sequence3[i] = stateInp->encoders[i+12] / 4;
	}
}



void Patch_updateSEQ (State* state) {

	updateClock (&clock0, 0.01);
	updateClock (&clock1, 0.01);
	updateClock (&clock2, 0.01);
	updateClock (&clock3, 0.01);

	if (clock0.v ==1 && clock0.lastV ==0) {
		cnt0 = (cnt0+1) % 4;
	}

	if (clock1.v ==1 && clock1.lastV ==0) {
		cnt1 = (cnt1+1) % 4;
	}

	if (clock2.v ==1 && clock2.lastV ==0) {
		cnt2 = (cnt2+1) % 4;
	}

	if (clock3.v ==1 && clock3.lastV ==0) {
		cnt3 = (cnt3+1) % 4;
	}



	state->cvOut[0] = PITCH_TABLE[sequence0[cnt0]]; //sequence[step] * 600;
	state->cvOut[1] = PITCH_TABLE[sequence1[cnt1]]; //sequence[step] * 600;
	state->cvOut[2] = PITCH_TABLE[sequence2[cnt2]]; //sequence[step] * 600;
	state->cvOut[3] = PITCH_TABLE[sequence3[cnt3]]; //sequence[step] * 600;

	state->trOut[0] = (int)clock0.v;
	state->trOut[1] = (int)clock1.v;
	state->trOut[2] = (int)clock2.v;
	state->trOut[3] = (int)clock3.v;

	state->encLeds = (1<<cnt0) | (1<<(cnt1+4)) | (1<<(cnt2+8)) | (1<<(cnt3+12));
}
