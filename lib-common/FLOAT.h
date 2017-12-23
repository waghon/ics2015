#ifndef __FLOAT_H__
#define __FLOAT_H__

#include "trap.h"

typedef int FLOAT;

static inline int F2int(FLOAT a) {
//	nemu_assert(0);	
	return a/0x10000;
}

static inline FLOAT int2F(int a) {
//	nemu_assert(0);
	return a*0x10000;
}

static inline FLOAT F_mul_int(FLOAT a, int b) {
//	nemu_assert(0);
	return a*b;
}

static inline FLOAT F_div_int(FLOAT a, int b) {
//	nemu_assert(0);
	int result=a/b;
	if((a%b)*2>=b)
		result++;
	return result;
}

FLOAT f2F(float);
FLOAT F_mul_F(FLOAT, FLOAT);
FLOAT F_div_F(FLOAT, FLOAT);
FLOAT Fabs(FLOAT);
FLOAT sqrt(FLOAT);
FLOAT pow(FLOAT, FLOAT);

#endif