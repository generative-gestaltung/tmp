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

Clock clock3 = {
	.v = 0.0,
	.lastV = 0.0,
	.ds = 0x1fff,
	.cnt = 0
};

Clock clock10 = {
	.v = 0.0,
	.lastV = 0.0,
	.ds = 0x1fff,
	.cnt = 0
};


int counter4 = 0;
int counter14 = 0;
int const8 = 0;
int const12 = 0;
int const5 = 0;
int const15 = 0;

int sequence[16] = { 0, 3, 7, 12,
                     0, 3, 7, 12,
		     5, 10, 17, 22,
                     5, 10, 17, 22};


void Patch_updateInpSEQ (StateInp* stateInp) {
	int i;
	for (i=0; i<16; i++) {
		sequence[i] = stateInp->encoders[i];
	}
}



void Patch_updateSEQ (State* state) {

	const8  = gui_params[8];
	const12 = gui_params[12];
	const5  = gui_params[5];
	const15 = gui_params[15];

	if (const5<1) const5 = 1;
	if (const8<1) const8 = 1;
	if (const12<1) const12 = 1;
	if (const15<1) const15 = 1;

	clock3.ds = FREQ_TABLE[127-const8];
	clock10.ds = clock3.ds * const12;

	updateClock (&clock3, 0.01);
	updateClock (&clock10, 0.01);


	if (clock3.v ==1 && clock3.lastV ==0) {
		counter4 = (counter4+1) % const5;
		printf("%d\n", counter4);
	}

	if (clock10.v ==1 && clock10.lastV ==0) {
		counter14 = (counter14+1) % const15;
	}


	state->cvOut[0] = PITCH_TABLE[sequence[counter4]]; //sequence[step] * 600;
	state->cvOut[1] = PITCH_TABLE[sequence[counter14]]; //sequence[step] * 600;

	state->trOut[0] = (int)clock3.v;
	state->trOut[0] = (int)clock10.v;

	//printf("%d\n", state->trOut[0]);
	state->encLeds = (1<<counter4);
}
