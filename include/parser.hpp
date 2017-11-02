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
class BaseNode;
inline void for_each_tag(std::wstring& _source, const wchar_t* pattern, std::function<bool(std::wstring& tag)> eachCB){
	
	std::wsmatch sm;
	std::wregex e(pattern);
	std::wstring source = _source;
	while (std::regex_search(source.cbegin(), source.cend(), sm, e))
	{
		std::wstring&& s = sm[0];
		eachCB(s);
		source = sm.suffix().str();
	}
}
	
class BaseNode {
public:
	bool parse(std::wstring source) {
		try {
			std::vector<BaseNode*> tmpPrePointer;
			tmpPrePointer.push_back(this);
			BaseNode* curent = this;
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
		std::wregex nameParse(L"<(\\S+)(.*)(.)>");
		std::wstring&& sss1 = std::regex_replace(sproperty, nameParse, L"$1", std::regex_constants::format_no_copy);
		std::wstring&& sss2 = std::regex_replace(sproperty, nameParse, L"$2", std::regex_constants::format_no_copy);
		std::wstring&& sss3 = std::regex_replace(sproperty, nameParse, L"$3", std::regex_constants::format_no_copy);

		name = sss1;
		is_single = sss3 == L"/";
		const wchar_t* pRegex = L"\\s+(.*?)=.*?\\\"(.+?)\\\"";
		std::wregex e(pRegex);
		for_each_tag(sss2, pRegex,[&](std::wstring& ref) {
			std::wstring&& key = std::regex_replace(ref, e, L"$1", std::regex_constants::format_no_copy);
			std::wstring&& val = std::regex_replace(ref, e, L"$2", std::regex_constants::format_no_copy);

			this->values[key] = val;
			return true;
		});
	}
	BaseNode* createChild(std::wstring& ref) {
		auto* pChild = new BaseNode();
		pChild->set(ref);
		childs.push_back({ pChild->name,std::unique_ptr<BaseNode>(pChild) });
		return pChild;
	}
	bool is_single = false;
protected:
	std::wstring name;
	std::map<std::wstring, std::wstring> values;
	std::vector<std::pair<std::wstring, std::unique_ptr<BaseNode>>> childs;
};

class DataXmlNode {
public:
	DataXmlNode(std::wstring&& source) {
		this->m_root.reset(new BaseNode());
		this->m_curent = this->m_root.get();
		this->m_root->parse(source);
	}
	DataXmlNode(DataXmlNode&& source) {
		m_root = source.m_root;
		m_curent = source.m_curent;
	}

	DataXmlNode& operator =(DataXmlNode&& source) {
		m_root.reset(new BaseNode());
		m_curent = source.m_curent;
	}
	std::wstring operator [](std::wstring) {

	}
private:
	std::shared_ptr<BaseNode> m_root;
	BaseNode* m_curent;
};


}

#endif //PARSER_HPP