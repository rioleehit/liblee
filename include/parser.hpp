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
enum ObjType {
	INT,
	FLOAT,
	BOOL,
	STRING,
	OBJECT,
	ARRAY,
	NIL,
	unkown
};

class DataJsonNode {
public:
	bool parseObjectFrom(std::wstring& source) {
		source = std::regex_replace(source, std::wregex(L"\\s*(\\S.*)"), L"$1", std::regex_constants::format_no_copy);
		std::wregex reFirst(L"\\{\\s*([\"\\}])?(.*)");
		std::wstring&& sFirst = std::regex_replace(source, reFirst, L"$1", std::regex_constants::format_no_copy);
		if (sFirst == L"}") {
			this->type = ObjType::NIL;
			source = std::regex_replace(source, reFirst, L"$2", std::regex_constants::format_no_copy);
			return true;
		}
		this->type = ObjType::OBJECT;
		std::wsmatch sm;
		std::wregex re(L"\"(.*?)\"?\\s*:\\s*(\\S.*)");
		std::wstring&& endStr=std::wstring(L"");
		while (std::regex_search(source, sm, re) && (endStr!=L"}"))
		{
			std::wstring&& searchStr = sm[0];
			std::unique_ptr<DataJsonNode>pChild(new DataJsonNode());
			pChild->name = std::regex_replace(searchStr, re, L"$1", std::regex_constants::format_no_copy);
			source = std::regex_replace(searchStr, re, L"$2", std::regex_constants::format_no_copy);
			wchar_t first = *source.c_str();

			switch (first)
			{
			case L'{': 
				pChild->parseObjectFrom(source);
				break;
			case L'[': pChild->parseArrayFrom(source);
				break;
			default:
			{
				std::wsmatch sm;
				std::wregex reProperty(L"(\\S*?)\\s*?([\\,\\}])\\s*(\\S.*)");
				std::wstring&& sub = std::regex_replace(source, reProperty, L"$1", std::regex_constants::format_no_copy);
				pChild->parseBaseTypeFrom(sub);
				source = std::move(std::regex_replace(source, reProperty, L"$2$3", std::regex_constants::format_no_copy));
			}
				break;
			}
			std::wregex reEnd(L"\\s*(\\S)\\s*(\\S.*)");
			endStr = std::move(std::regex_replace(source, reEnd, L"$1", std::regex_constants::format_no_copy));
			source = std::move(std::regex_replace(source, reEnd, L"$2", std::regex_constants::format_no_copy));

			this->childs.push_back({ pChild->name,std::move(pChild)});
		}
		return true;
	}
	bool parseArrayFrom(std::wstring& source) {

		std::wregex reEach(L"\\s*(\\S)(.*)");
		std::wregex reBase(L"\\s*(\\S*?)\\s*([,\\]].*)");
		source = std::regex_replace(source, std::wregex(L"\\s*\\[?(.*)"), L"$1", std::regex_constants::format_no_copy);
		this->type = ObjType::ARRAY;
		int index = 0;
		bool loop = true;
		while (loop) {
			std::wstring&& next = std::regex_replace(source, reEach, L"$1", std::regex_constants::format_no_copy);
			source = std::regex_replace(source, reEach, L"$1$2", std::regex_constants::format_no_copy);
			
			std::unique_ptr<DataJsonNode> pChild(new DataJsonNode());
			switch (*next.c_str())
			{
			case L']':source = (source.c_str() + 1); loop = false;
				break;
			case L',':source = (source.c_str() + 1); continue;
				break;
			case L'[':pChild->parseArrayFrom(source);
					  this->childs.push_back({ std::to_wstring(index),std::move(pChild) });
					  ++index;
				break;
			case L'{':pChild->parseObjectFrom(source);
					  this->childs.push_back({ std::to_wstring(index),std::move(pChild) });
					  ++index;
				break;			
			default:
					{
						std::wsmatch sm;
						if (std::regex_search(source, sm, reBase))
						{
							std::wstring&& searchStr = sm[0];
							std::wstring&& s = std::regex_replace(searchStr, reBase, L"$1", std::regex_constants::format_no_copy);
							source = std::regex_replace(searchStr, reBase, L"$2", std::regex_constants::format_no_copy);
							pChild->parseBaseTypeFrom(s);
							this->childs.push_back({ std::to_wstring(index),std::move(pChild) });
							++index;
						}
					}break;
			}
		}
		return true;
	}
	bool parseBaseTypeFrom(std::wstring& source) {
		wchar_t first = *source.c_str();
		switch (first) {
			case L'\"':
				this->type = ObjType::STRING;
				this->value = source.substr(1, source.length() - 2);
				break;
			case L'n':
			case L'N':
				this->type = ObjType::NIL;
				break;
			case L't':
			case L'T':
				this->value = L"true";
				this->type = ObjType::BOOL;
				break;
			case L'f':
			case L'F':
				this->value = L"false";
				this->type = ObjType::BOOL; 
				break;
			default:
				this->type = source.find(L'.') != std::string::npos ? ObjType::FLOAT : ObjType::INT;
				this->value = source;
		}
		return true;
	}
	static DataJsonNode* parse(std::wstring& source) {
		DataJsonNode* result = new DataJsonNode();
		try {
			std::wregex regObjType(L"\\s*(\\S.*)");
			std::wstring&& snext = std::regex_replace(source, regObjType, L"$1", std::regex_constants::format_no_copy);
			wchar_t stype = *snext.c_str();
			
			result->type = ObjType::unkown;
			DataJsonNode* pChild = new DataJsonNode();
			std::function<bool(std::wstring&)> parser;
			switch (stype)
			{
			case L'[':
				parser = std::bind(&(DataJsonNode::parseArrayFrom), result, std::placeholders::_1);
				break;
			case L'{':
				parser = std::bind(&(DataJsonNode::parseObjectFrom), result, std::placeholders::_1);
				break;
			default:
				parser = std::bind(&(DataJsonNode::parseBaseTypeFrom), result, std::placeholders::_1);
				break;
			}
			parser(snext);
		}
		catch (...) { }
		return result;
	}

	bool isEnd(std::wstring& s) {
		std::wstring temp = s.substr(2, s.length() - 3);
		wchar_t c = *(s.c_str() + 1);
		return (c == L'/') && (name.compare(temp) == 0);
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
		for_each_tag(sss2, pRegex, [&](std::wstring& ref) {
			std::wstring&& key = std::regex_replace(ref, e, L"$1", std::regex_constants::format_no_copy);
			std::wstring&& val = std::regex_replace(ref, e, L"$2", std::regex_constants::format_no_copy);

			//this->values[key] = val;
			return true;
		});
	}
	DataJsonNode* createChild(std::wstring& ref) {
		auto* pChild = new DataJsonNode();
		pChild->set(ref);
		childs.push_back({ pChild->name,std::unique_ptr<DataJsonNode>(pChild) });
		return pChild;
	}
	bool is_single = false;
	DataJsonNode():length(0),type(ObjType::unkown),name(),value(),childs(){}
protected:
	ObjType type;
	std::wstring name;
	std::wstring value;
	int length;
	std::vector<std::pair<std::wstring, std::unique_ptr<DataJsonNode>>> childs;
};
class DataXmlNode {
public:
	bool parse(std::wstring source) {
		try {
			std::vector<DataXmlNode*> tmpPrePointer;
			tmpPrePointer.push_back(this);
			DataXmlNode* curent = this;
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
		std::wstring temp = s.substr(2, s.length() - 3);
		wchar_t c = *(s.c_str() + 1);
		return (c == L'/') && (name.compare(temp) == 0);
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
		for_each_tag(sss2, pRegex, [&](std::wstring& ref) {
			std::wstring&& key = std::regex_replace(ref, e, L"$1", std::regex_constants::format_no_copy);
			std::wstring&& val = std::regex_replace(ref, e, L"$2", std::regex_constants::format_no_copy);

			this->values[key] = val;
			return true;
		});
	}
	DataXmlNode* createChild(std::wstring& ref) {
		auto* pChild = new DataXmlNode();
		pChild->set(ref);
		childs.push_back({ pChild->name,std::unique_ptr<DataXmlNode>(pChild) });
		return pChild;
	}
	bool is_single = false;
protected:
	std::wstring name;
	std::map<std::wstring, std::wstring> values;
	std::vector<std::pair<std::wstring, std::unique_ptr<DataXmlNode>>> childs;
};
class Object {
public:
	Object(std::wstring source) {

	}
private:
	std::shared_ptr<BaseNode> m_root;
	BaseNode* m_curent;

};


}

#endif //PARSER_HPP