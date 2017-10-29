#ifndef PARSER_HPP
#define PARSER_HPP

#include <locale>
#include <string>
#include <codecvt>
#include <vector>
#include <memory>
#include <map>
#include <regex>  

namespace lee{
class DataNode;
inline void for_each_tag(std::wstring& _source, const wchar_t* pattern, std::function<bool(std::wstring& tag)> eachCB){
	
	std::wsmatch sm;
	std::wregex e(pattern);
	//std::regex_match(source, sm, e);
	std::wstring source = _source;
	//std::wsmatch::iterator x;
	//for (x = sm.begin(); x != sm.end(); ++x) {
	//	//cout << x->str() << endl;
	//	eachCB(x->str());
	//}
	//return;
	while (std::regex_search(source.cbegin(), source.cend(), sm, e))
	{
		std::wstring&& s = sm[0];
		eachCB(s);
		source = sm.suffix().str();
	}
}
	
class DataNode {
public:
	bool parse(std::wstring source) {
		try {
			std::vector<DataNode*> tmpPrePointer;
			tmpPrePointer.push_back(this);
			DataNode* curent = this;
			int index = 0;
			for_each_tag(source, L"<(\\S*?)[^>]*?>.*?|<.*? />", [&](std::wstring& ref)->bool {
				bool isSingle = curent->is_single;
				if (isSingle || curent->isEnd(ref)) {
					tmpPrePointer.pop_back();
					--index;
					curent = tmpPrePointer[index];
					if (!isSingle || curent->isEnd(ref)) { return true; }
				}				
				curent = curent->createChild(ref);
				tmpPrePointer.push_back(curent);
				++index;
				return true; 
			});
		}
		catch (...) {
			return false; 
		}
		return true;
	}
	bool isEnd(std::wstring& s) {
		std::wstring temp = s.substr(2,s.length()-3);
		wchar_t c = *(s.c_str() + 1);
		return (c == L'/')&&(name.compare(temp)==0);
	}
	void set(std::wstring& sproperty) {
		int i = sproperty.find_first_of(L" ");
		name = sproperty.substr(1, i<0? sproperty.length()-2 : i);
		name.erase(0, name.find_first_not_of(L" "));
		name.erase(name.find_last_not_of(L" ") + 1);
		i = sproperty.length() - 2;
		is_single = sproperty.substr(i, 1) == L"/";
		for_each_tag(sproperty, L" .*?=.*?\\\".+?\\\"", [&](std::wstring& ref) {
			std::wstring&& key = ref.substr(ref.find_first_of(L" "), ref.find(L"="));
			key.erase(0, key.find_first_not_of(L" "));
			key.erase(key.find_last_not_of(L" ") + 1);
			int begin = ref.find_first_of(L"\"") + 1;
			std::wstring&& val = ref.substr(begin, ref.find_last_of(L"\"")-begin);
			this->values[key] = val;
			return true;
		});
	}
	DataNode* createChild(std::wstring& ref) {
		auto* pChild = new DataNode();
		pChild->set(ref);
		childs.push_back({ pChild->name,std::unique_ptr<DataNode>(pChild) });
		return pChild;
	}
	bool is_single = false;
private:
	std::wstring name;
	std::map<std::wstring, std::wstring> values;
	std::vector<std::pair<std::wstring, std::unique_ptr<DataNode>>> childs;
};

}

#endif //PARSER_HPP