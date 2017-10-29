#ifndef UTIL_HPP
#define UTIL_HPP

#include <locale>
#include <string>
#include <codecvt>
#include <sstream>

namespace lee{
	//std::u16string to_utf16(std::string str) // utf-8 to utf16
	//{
	//	return std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.from_bytes(str);
	//}

	//std::string to_utf8(std::u16string str16)
	//{
	//	return std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.to_bytes(str16);
	//}

	//std::u32string to_utf32(std::string str)
	//{
	//	return std::wstring_convert< std::codecvt_utf8<char32_t>, char32_t >{}.from_bytes(str);
	//}

	//std::string to_utf8(std::u32string str32)
	//{
	//	return std::wstring_convert< std::codecvt_utf8<char32_t>, char32_t >{}.to_bytes(str32);
	//}

	std::wstring to_wchar_t(std::string str)
	{
		return std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.from_bytes(str);
	}

	std::string to_utf8(std::wstring wstr)
	{
		return std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.to_bytes(wstr);
	}
inline std::string CvtString(const wchar_t* source){
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conversion;
	return conversion.to_bytes(source);
}
inline std::wstring CvtString(const char* source){
	try
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(source);
	}
	catch (std::range_error& e)
	{
		size_t length = strlen(source);//s input.length();
		std::wstringstream result;
		for (size_t i = 0; i < length; i++)
		{
			result << (wchar_t)(source[i] & 0xFF);
		}
		return result.str();
	}
}
inline std::string CvtString(std::wstring& source){ return CvtString(source.c_str()); }
inline std::wstring CvtString(std::string& source){ return CvtString(source.c_str()); }
	
}

#endif //UTIL_HPP