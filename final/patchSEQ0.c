#include "patch.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define BASE_SPEED  0x0fff



static Clock clock0 = {
	.v = 0.0,
	.lastV = 0.0,
	.ds = BASE_SPEED,
	.cnt = 0
};


static Clock clock1 = {
	.v = 0.0,
	.lastV = 0.0,
	.ds = BASE_SPEED*2,
	.cnt = 0
};

static Clock clock2 = {
	.v = 0.0,
	.lastV = 0.0,
	.ds = BASE_SPEED*3,
	.cnt = 0
};

static Clock clock3 = {
	.v = 0.0,
	.lastV = 0.0,
	.ds = BASE_SPEED*4,
	.cnt = 0
};


static int cnt0 = 0;
static int cnt1 = 0;
static int cnt2 = 0;
static int cnt3 = 0;

static int sequence0[4] = { 0, 3, 7, 12 };
static int sequence1[4] = { 0, 3, 7, 12 };
static int sequence2[4] = { 0, 3, 7, 12 };
static int sequence3[4] = { 0, 3, 7, 12 };

static int baseSpeed = BASE_SPEED;

void Patch_updateInpSEQ0 (StateInp* stateInp) {
	int i;
	for (i=0; i<4; i++) {
		sequence0[i] = stateInp->encoders[i] / 4;
		sequence1[i] = stateInp->encoders[i+4] / 4;
		sequence2[i] = stateInp->encoders[i+8] / 4;
		sequence3[i] = stateInp->encoders[i+12] / 4;
	}

	baseSpeed = FREQ_TABLE[127-gui_params[6]%128];

	clock0.ds = baseSpeed * gui_params[7];
	clock1.ds = baseSpeed * gui_params[8];
	clock2.ds = baseSpeed * gui_params[9];
	clock3.ds = baseSpeed * gui_params[10];
}



void Patch_updateSEQ0 (State* state) {

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
