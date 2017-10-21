#ifndef GET_SET_HPP
#define GET_SET_HPP
#include <string>
#include <functional>
namespace lee{
	
#define T_REF const T&
template<class T>
class Get {
public:
	Get(std::function<T_REF()> get) :m_get(get) {}
	operator T_REF() { return m_get(); }
	std::string toString() { return std::to_string(m_get()); }
protected:
	//Get(Get) {}
	std::function<T_REF()> m_get;
};
template<>
class Get <std::string>{
public:
	Get(std::function<const std::string&()> get) :m_get(get) {}
	operator const std::string&() { return m_get(); }
	std::string toString() { return m_get(); }
protected:
	//Get(Get) {}
	std::function<const std::string&()> m_get;
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
	void operator=(T v) { Set<T>::operator=(v); }
	operator T_REF() { return  Get<T>::m_get(); }
	GetSet(std::function<T_REF()> get) :Get<T>(get){}
	GetSet(std::function<void(T)> set) :Set<T>(set) {}
	GetSet(std::function<T_REF()> get, std::function<void(T)> set) :Get<T>(get), Set<T>(set) {}
	GetSet(std::function<void(T)> set, std::function<T_REF()> get) :Get<T>(get), Set<T>(set) {}
};

};
#undef T_REF
#define GetVal [this]()
#define GetRef(type) [this]()->const type&
#define SetVal(type) [this](type value)
#endif //GET_SET_HPP