// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2014. LH_Mouse. All wrongs reserved.

#ifndef MCF_STD_MCF_HPP_
#define MCF_STD_MCF_HPP_

#ifdef UNICODE
#	define _UNICODE			1
#endif

#ifndef NDEBUG
#	define _GLIBCXX_DEBUG	1
#endif

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <memory>
#include <algorithm>
#include <limits>
#include <utility>
#include <functional>
#include <type_traits>

#include "../MCFCRT/MCFCRT.h"
#include "../MCFCRT/env/mcfwin.h"

#undef NULL
#define NULL	nullptr

namespace MCF {
}

#endif
