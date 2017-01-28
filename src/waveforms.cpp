#include "waveforms.h"
#include <math.h>
#include <stdlib.h>
#define LOOP_MOD_F(x,y) (fmod((fmod((x),(y))+(y)),(y)))

float sawFunction (float x){
    return LOOP_MOD_F((x-.5f)*2,2.f)-1.f;
}

float triangleFunction (float x){
    float xt = LOOP_MOD_F(x-0.25f,1.f);
    return (xt<0.5f)?
            (1.f - xt*4.f):
            (xt*4.f - 3.f);
}

float squareFunction (float x){
    float xt = LOOP_MOD_F(x,1.f);
    return (xt>0.5)?
            (1.f):
            (-1.f);
}

float rectangleFunction (float x, float pulse){
    float xt = LOOP_MOD_F(x,1.f);
    return (xt>pulse)?
            (1.f):
            (-1.f);
}

float sineFunction (float x){
    float xt = LOOP_MOD_F(x,1.f);
    return sin(xt*M_PI*2);
}

float zeroFunction (float x){
    return 0.f;
}

float loopModFunction (float x){
	return LOOP_MOD_F(x,1.f);
}

float clampFunction (float x){
	if(x>1.)
		return 1;
	else if(x<0.)
		return 0;
	else
		return x;
}
