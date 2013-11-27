// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013. LH_Mouse. All wrongs reserved.

#include "../../env/_crtdef.h"
#include <wchar.h>

__MCF_CRT_EXTERN size_t wcsspn(const wchar_t *s1, const wchar_t *s2){
	const wchar_t *rp = s1;
	for(;;){
		const wchar_t ch = *rp;
		if(ch == 0){
			break;
		}
		if(wcschr(s2, ch) == NULL){
			break;
		}
		++rp;
	}
	return (size_t)(rp - s1);
}
