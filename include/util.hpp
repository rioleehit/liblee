#ifndef UTIL_HPP
#define UTIL_HPP

#include <locale>
#include <string>
#include <codecvt>

namespace lee{

inline std::string CvtString(const wchar_t* source){
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> conversion;
	return conversion.to_bytes(source);
}
inline std::wstring CvtString(const char* source){
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> conversion;
	return conversion.from_bytes(source);
}
inline std::string CvtString(std::wstring& source){ return CvtString(source.c_str()); }
inline std::wstring CvtString(std::string& source){ return CvtString(source.c_str()); }
	
}

#endif //UTIL_HPP