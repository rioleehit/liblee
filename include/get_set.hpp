#ifndef GET_SET_HPP
#define GET_SET_HPP
#include <string>
#include <functional>
namespace lee{
	
#define T_REF const T&

template<class T>
class Set {
public:
	Set(std::function<void(const T&)>& set) :m_set(set) {}
	Set(std::function<void(const T&)>&& set) :m_set(set) {}
	void operator=(const T& v) { m_set(v); }
	void operator=(const T&& v) { m_set(v); }
protected:
	Set(const Set&) = delete;
	std::function<void(const T&)> m_set;
};

template<class T>
class Get {
public:
	Get(std::function<T()>& get) :m_get(get) {}
	Get(std::function<T()>&& get) :m_get(get) {}
	T val() { return m_get(); }
	operator T() { return m_get(); }
	std::string toString() { return std::to_string(m_get()); }
protected:
	Get(const Get&) {}
	std::function<T()> m_get;
};
template<>
class Get <std::string>{
public:
	Get(std::function<std::string()> get) :m_get(get) {}
	std::string val() { return m_get(); }
	operator std::string() { return m_get(); }
	std::string toString() { return m_get(); }
protected:
	Get(const Get&) {}
	std::function<const std::string()> m_get;
};

template<class T>
class RefGet:public Get<T&>{
public:
	RefGet(std::function<T&()> f) :Get<T&>(f) {}
	T& ref() { return Get<T&>::val(); };
	T& val() = delete;
private:
	RefGet(const RefGet&) {}
};
template<class T>
class GetSet :public Set<T>, public Get<T> {
public:
	void operator=(const T& v) { m_set(v); }
	void operator=(const T&& v) { m_set(v); }
	GetSet(std::function<T()> get, std::function<void(const T&)> set) :Get<T>(get), Set<T>(set) {}
	GetSet(std::function<void(const T&)> set, std::function<T()> get) :Get<T>(get), Set<T>(set) {}
};

template<class T>
class RefGetSet :public Set<T>, public Get<T&> {
public:
	void operator=(const T& v) { m_set(v); }
	void operator=(const T&& v) { m_set(v); }
	RefGetSet(std::function<T&()> get, std::function<void(const T&)> set) :Get<T&>(get), Set<T>(set) {}
	RefGetSet(std::function<void(const T&)> set, std::function<T&()> get) :Get<T&>(get), Set<T>(set) {}
};

};
#undef T_REF
#define GetVal [this]()
#define GetRef(type) [this]()->type&
//#define SetVal(type) [this](type value)
#define SetRef(type) [this](const type& value)
#endif //GET_SET_HPP