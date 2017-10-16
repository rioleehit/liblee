#ifndef GET_SET_HPP
#define GET_SET_HPP
#include <string>
#include <functional>
namespace lee{
	

template<class T>
class Get {
public:
	Get(std::function<T()> get) :m_get(get) {}
	operator T() { return m_get(); }

	std::string toString() { return std::to_string(m_get()); }
protected:
	//Get(Get) {}
	std::function<T()> m_get;
};
template<>
class Get <std::string>{
public:
	Get(std::function<std::string()> get) :m_get(get) {}
	operator std::string() { return m_get(); }

	std::string toString() { return m_get(); }
protected:
	//Get(Get) {}
	std::function<std::string()> m_get;
};

template<class T>
class Set {
public:
	Set(std::function<void(T)> set) :m_set(set) {}
	void operator=(T v){ m_set(v); }
protected:
	//Set(Set) {}
	std::function<void(T)> m_set;
};
	
template<class T>
class GetSet :public Set<T>, public Get<T> {
public:
	void operator=(T v) { Set::operator=(v); }
	operator T() { return  m_get(); }
	GetSet(std::function<T()> get) :Get<T>(get){}
	GetSet(std::function<void(T)> set) :Set<T>(set) {}
	GetSet(std::function<T()> get, std::function<void(T)> set) :Get<T>(get), Set<T>(set) {}
	GetSet(std::function<void(T)> set, std::function<T()> get) :Get<T>(get), Set<T>(set) {}
};

};
#define GetVal [this]()
#define SetVal(type) [this](type value)
#endif //GET_SET_HPP