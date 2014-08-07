// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2014, LH_Mouse. All wrongs reserved.

#ifndef MCF_LOCK_RAII_TEMPLATE_HPP_
#define MCF_LOCK_RAII_TEMPLATE_HPP_

#include "../Core/Utilities.hpp"
#include <cstddef>

namespace MCF {

class LockRaiiTemplateBase : NO_COPY, ABSTRACT {
protected:
	std::size_t xm_uLockCount;

protected:
	constexpr LockRaiiTemplateBase() noexcept
		: xm_uLockCount(0)
	{
	}

public:
	virtual ~LockRaiiTemplateBase() noexcept {
		ASSERT(xm_uLockCount == 0);
	}

private:
	virtual bool xDoTry() const noexcept = 0;
	virtual void xDoLock() const noexcept = 0;
	virtual void xDoUnlock() const noexcept = 0;

public:
	bool IsLocking() const noexcept {
		return xm_uLockCount > 0;
	}
	bool Try(std::size_t uCount = 1) noexcept {
		if(xm_uLockCount == 0){
			if(!xDoTry()){
				return false;
			}
		}
		xm_uLockCount += uCount;
		return true;
	}
	void Lock(std::size_t uCount = 1) noexcept {
		if(uCount == 0){
			return;
		}
		if(xm_uLockCount == 0){
			xDoLock();
		}
		xm_uLockCount += uCount;
	}
	void Unlock(std::size_t uCount = 1) noexcept {
		if(uCount == 0){
			return;
		}

		ASSERT(xm_uLockCount >= uCount);

		xm_uLockCount -= uCount;
		if(xm_uLockCount == 0){
			xDoUnlock();
		}
	}
	std::size_t UnlockAll() noexcept {
		const auto uCount = std::exchange(xm_uLockCount, 0u);
		if(uCount > 0){
			xDoUnlock();
		}
		return uCount;
	}

public:
	explicit operator bool() const noexcept {
		return IsLocking();
	}
};

namespace Impl {
	template<class Mutex, std::size_t LOCK_TYPE = 0>
	class LockRaiiTemplate : CONCRETE(LockRaiiTemplateBase) {
	private:
		Mutex *xm_pOwner;

	public:
		explicit LockRaiiTemplate(Mutex *pOwner, std::size_t uInitCount = 1) noexcept
			: xm_pOwner(pOwner)
		{
			Lock(uInitCount);
		}
		LockRaiiTemplate(LockRaiiTemplate &&rhs) noexcept
			: xm_pOwner(rhs.xm_pOwner)
		{
			xm_uLockCount = std::exchange(rhs.xm_uLockCount, 0u);
		}
		LockRaiiTemplate &operator=(LockRaiiTemplate &&rhs) noexcept {
			if(this != &rhs){
				UnlockAll();

				xm_pOwner = rhs.xm_pOwner;
				xm_uLockCount = std::exchange(rhs.xm_uLockCount, 0u);
			}
			return *this;
		}
		virtual ~LockRaiiTemplate() noexcept {
			UnlockAll();
		}

	private:
		bool xDoTry() const noexcept override;
		void xDoLock() const noexcept override;
		void xDoUnlock() const noexcept override;

	public:
		void Join(LockRaiiTemplate &&rhs) noexcept {
			ASSERT(xm_pOwner == rhs.xm_pOwner);

			xm_uLockCount += std::exchange(rhs.xm_uLockCount, 0u);
		}

		void Swap(LockRaiiTemplate &rhs) noexcept {
			std::swap(xm_pOwner, rhs.xm_pOwner);
			std::swap(xm_uLockCount, rhs.xm_uLockCount);
		}
	};
}

}

#endif
