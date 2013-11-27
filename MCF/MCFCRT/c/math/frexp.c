// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013. LH_Mouse. All wrongs reserved.

#include "../../env/_crtdef.h"
#include "_mathasm.h"

static const double HALF = 0.5;

__MCF_CRT_EXTERN float frexpf(float x, int *exp){
	register float ret;
	__asm__ __volatile__(
		"fld dword ptr[%1] \n"
		"ftst \n"
		"fstsw ax \n"
		"test ah, 0x40 \n"
		"jz 1f \n"
		"	fldz \n"
		"	jmp 2f \n"
		"1: \n"
		"fxtract \n"
		"fmul qword ptr[%3] \n"
		"fld1 \n"
		"faddp st(2), st \n"
		"fxch st(1) \n"
		"2: \n"
		"fistp dword ptr[%2] \n"
		__FLT_RET_ST("%1")
		: __FLT_RET_CONS(ret)
		: "m"(x), "r"(exp), "m"(HALF)
		: "eax"
	);
	return ret;
}

__MCF_CRT_EXTERN double frexp(double x, int *exp){
	register double ret;
	__asm__ __volatile__(
		"fld qword ptr[%1] \n"
		"ftst \n"
		"fstsw ax \n"
		"test ah, 0x40 \n"
		"jz 1f \n"
		"	fldz \n"
		"	jmp 2f \n"
		"1: \n"
		"fxtract \n"
		"fmul qword ptr[%3] \n"
		"fld1 \n"
		"faddp st(2), st \n"
		"fxch st(1) \n"
		"2: \n"
		"fistp dword ptr[%2] \n"
		__DBL_RET_ST("%1")
		: __DBL_RET_CONS(ret)
		: "m"(x), "r"(exp), "m"(HALF)
		: "eax"
	);
	return ret;
}

__MCF_CRT_EXTERN __LDBL_DECL(frexpl, long double x, int *exp){
	register long double ret;
	__asm__ __volatile__(
		"fld tbyte ptr[%1] \n"
		"ftst \n"
		"fstsw ax \n"
		"test ah, 0x40 \n"
		"jz 1f \n"
		"	fldz \n"
		"	jmp 2f \n"
		"1: \n"
		"fxtract \n"
		"fmul qword ptr[%3] \n"
		"fld1 \n"
		"faddp st(2), st \n"
		"fxch st(1) \n"
		"2: \n"
		"fistp dword ptr[%2] \n"
		__LDBL_RET_ST()
		: __LDBL_RET_CONS(ret)
		: "m"(x), "r"(exp), "m"(HALF), __LDBL_RET_CONS_IN()
		: "eax"
	);
	__LDBL_RETURN(ret);
}
