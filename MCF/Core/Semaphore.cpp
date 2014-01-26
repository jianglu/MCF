// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2012 - 2013. LH_Mouse. All wrongs reserved.

#include "../StdMCF.hpp"
#include "Semaphore.hpp"
#include "Exception.hpp"
#include "UniqueHandle.hpp"
using namespace MCF;

// 嵌套类定义。
class Semaphore::xDelegate : NO_COPY {
private:
	struct xSemaphoreCloser {
		constexpr HANDLE operator()() const {
			return NULL;
		}
		void operator()(HANDLE hSemaphore) const {
			::CloseHandle(hSemaphore);
		}
	};
private:
	UniqueHandle<HANDLE, xSemaphoreCloser> xm_hSemaphore;
public:
	xDelegate(std::size_t uInitCount, std::size_t uMaxCount, const wchar_t *pwszName){
		ASSERT((uInitCount <= LONG_MAX) && (uMaxCount <= LONG_MAX));

		xm_hSemaphore.Reset(::CreateSemaphoreW(nullptr, (long)uInitCount, (long)uMaxCount, pwszName));
		if(!xm_hSemaphore){
			MCF_THROW(::GetLastError(), L"CreateSemaphoreW() 失败。");
		}
	}
public:
	HANDLE GetHandle() const noexcept {
		return xm_hSemaphore;
	}
};

// 构造函数和析构函数。
Semaphore::Semaphore(std::size_t uInitCount, std::size_t uMaxCount, const wchar_t *pwszName)
	: xm_pDelegate(new xDelegate(uInitCount, uMaxCount, pwszName))
{
}
Semaphore::~Semaphore(){
}

// 其他非静态成员函数。
std::size_t Semaphore::WaitTimeOut(unsigned long ulMilliSeconds, std::size_t uWaitCount) noexcept {
	std::size_t uSucceeded = 0;
	if(ulMilliSeconds == INFINITE){
		for(std::size_t i = 0; i < uWaitCount; ++i){
			::WaitForSingleObject(xm_pDelegate->GetHandle(), INFINITE);
			++uSucceeded;
		}
	} else {
		const auto ulWaitUntil = ::GetTickCount() + ulMilliSeconds;
		unsigned long ulTimeToWait = ulMilliSeconds;
		for(std::size_t i = 0; i < uWaitCount; ++i){
			if(::WaitForSingleObject(xm_pDelegate->GetHandle(), ulMilliSeconds) == WAIT_TIMEOUT){
				break;
			}
			++uSucceeded;
			ulTimeToWait = ulWaitUntil - ::GetTickCount();
			if((long)ulTimeToWait < 0){
				break;
			}
		}
	}
	return uSucceeded;
}
void Semaphore::Wait(std::size_t uWaitCount) noexcept {
	WaitTimeOut(INFINITE, uWaitCount);
}
void Semaphore::Signal(std::size_t uSignalCount) noexcept {
	if(!::ReleaseSemaphore(xm_pDelegate->GetHandle(), uSignalCount, nullptr)){
		ASSERT_MSG(false, L"ReleaseSemaphore() 失败。");
	}
}
