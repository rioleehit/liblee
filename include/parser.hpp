#ifndef PARSER_HPP
#define PARSER_HPP

#include <locale>
#include <string>
#include <codecvt>
#include <vector>
#include <memory>
#include <map>

#include <regex.h>

namespace lee{

inline void for_each_tag(std::string& source, std::function<bool(std::string& tag)>& eachCB){	
    static const wchar_t* pattern = L"<(\S*?)[^>]*>.*?|<.*? />";
	
	
}
	
	
class DataNode {
public:
	bool parse(std::wstring) {
		try {

		}
		catch () { return false; }
		return true;
	}
private:
	std::map<std::wstring, std::wstring> values;
	std::map<std::wstring, std::unique_ptr<DataNode>> childs;
};

}

#endif //PARSER_HPP