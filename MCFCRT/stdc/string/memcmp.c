// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2015, LH_Mouse. All wrongs reserved.

#include "../../env/_crtdef.h"

#ifdef _WIN64
#	define BSWAP_PTR(val)	__builtin_bswap64(val)
#else
#	define BSWAP_PTR(val)	__builtin_bswap32(val)
#endif

int memcmp(const void *p1, const void *p2, size_t cb){
	const unsigned char *rp1 = (const unsigned char *)p1;
	const unsigned char *rp2 = (const unsigned char *)p2;

	size_t wcnt = cb / sizeof(uintptr_t);
	++wcnt;
	for(;;){

#define UNROLLED(idx_)	\
		{	\
			if(--wcnt == 0){	\
				rp1 += (idx_) * sizeof(uintptr_t);	\
				rp2 += (idx_) * sizeof(uintptr_t);	\
				break;	\
			}	\
			const uintptr_t wrd1 = ((const uintptr_t *)rp1)[idx_];	\
			const uintptr_t wrd2 = ((const uintptr_t *)rp2)[idx_];	\
			if(wrd1 != wrd2){	\
				return (BSWAP_PTR(wrd1) > BSWAP_PTR(wrd2)) ? 1 : -1;	\
			}	\
		}

		UNROLLED(0)
		UNROLLED(1)
		UNROLLED(2)
		UNROLLED(3)
		UNROLLED(4)
		UNROLLED(5)
		UNROLLED(6)
		UNROLLED(7)

		rp1 += 8 * sizeof(uintptr_t);
		rp2 += 8 * sizeof(uintptr_t);
	}
	size_t rem = cb % sizeof(uintptr_t);
	while(rem-- != 0){
		const int delta = (int)(unsigned char)*(rp1++) - (int)(unsigned char)*(rp2++);
		if(delta != 0){
			return (delta >> (sizeof(int) * __CHAR_BIT__ - 1)) | 1;
		}
	}
	return 0;
}
