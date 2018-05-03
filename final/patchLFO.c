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

Lfo lfo1 = {
	.p = 0.0,
	.f = 0.1,
	.A = 1.0,
	.v = 0.0,
	.w = WAVE_SIN
};

Lfo lfo2 = {
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




void Patch_updateInpLFO (StateInp* stateInp) {

	//memcpy (&_stateInp, state, sizeof(StateInp));
	lfo0.f = stateInp->encoders[0] / 255.;
	lfo0.A = stateInp->encoders[4] / 255.;
	lfo0.w = (int)(stateInp->encoders[8] / 32);

	lfo1.f = stateInp->encoders[1] / 255.;
	lfo1.A = stateInp->encoders[5] / 255.;
	lfo1.w = (int)(stateInp->encoders[9] / 32);

	lfo2.f = stateInp->encoders[2] / 255.;
	lfo2.A = stateInp->encoders[6] / 255.;
	lfo2.w = (int)(stateInp->encoders[10] / 32);

	rand0.ds = stateInp->encoders[3]+1;
	rand0.A = stateInp->encoders[7] / 255.;

}


void Patch_updateLFO (State* state) {

	updateLfo (&lfo0, 0.01);
	updateLfo (&lfo1, 0.01);
	updateLfo (&lfo2, 0.01);

	updateRand (&rand0, 0.01);

	state->cvOut[0] = (int)(lfo0.v * 65335);
	state->cvOut[1] = (int)(lfo1.v * 65335);
	state->cvOut[2] = (int)(lfo2.v * 65335);
	state->cvOut[3] = (int)(rand0.v * 0xffff);

	state->encLeds = 0x5555;
}
