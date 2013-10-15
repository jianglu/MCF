// Copyleft 2013, LH_Mouse. All wrongs reserved.

#include "PreCompiled.hpp"
#include <string>
using namespace MCFBuild;

namespace MCFBuild {
	void FixPath(std::wstring &wcsSrc){
		if(!wcsSrc.empty()){
			const DWORD dwSize = ::GetFullPathNameW(wcsSrc.c_str(), 0, nullptr, nullptr);
			std::wstring wcsTemp(dwSize, 0);
			wcsTemp.resize(::GetFullPathNameW(wcsSrc.c_str(), wcsTemp.size(), &wcsTemp[0], nullptr));
			wcsSrc = std::move(wcsTemp);
		}
	}

	std::wstring GetFileExtension(const std::wstring &wcsPath){
		std::wstring::const_iterator iterPrev = wcsPath.cend();
		auto iter = iterPrev;
		while(iter != wcsPath.cbegin()){
			switch(*--iterPrev){
			case L'.':
				return std::wstring(iter, wcsPath.cend());
			case L'\\':
			case L' ':
				break;
			}
			iter = iterPrev;
		}
		return std::wstring();
	}

	std::wstring MakeCommandLine(
		const std::wstring &wcsBase,
		const std::map<std::wstring, std::wstring> &mapReplacements,
		const wchar_t *pwszPrefix
	){
		std::wstring wcsRet;

		auto iterRead = wcsBase.cbegin();
		while(iterRead != wcsBase.cend()){
			const wchar_t ch = *(iterRead++);
			if(ch != L'%'){
				wcsRet.push_back(ch);
				continue;
			}

			auto iterNameEnd = iterRead;
			for(;;){
				if(iterNameEnd == wcsBase.cend()){
					throw Exception(ERROR_INVALID_DATA, L"“" + wcsBase + L"”中的 % 不匹配。");
				}
				if(*iterNameEnd == L'%'){
					break;
				}
				++iterNameEnd;
			}
			if(iterNameEnd == iterRead){
				wcsRet.push_back(L'%');
				++iterRead;
				continue;
			}

			const std::wstring wcsMacroName(iterRead, iterNameEnd);
			iterRead = ++iterNameEnd;

			const auto iterMacro = mapReplacements.find(wcsMacroName);
			if(iterMacro == mapReplacements.cend()){
				Error((pwszPrefix == nullptr) ? std::wstring() : std::wstring(pwszPrefix) + L"警告：宏“" + wcsMacroName + L"”没有定义，已替换为空字符串。");
				continue;
			}

			wcsRet.push_back(L' ');
			wcsRet.append(iterMacro->second);
			wcsRet.push_back(L' ');
		}

		return std::move(wcsRet);
	}
}
