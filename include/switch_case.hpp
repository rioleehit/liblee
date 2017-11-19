#ifndef SWITCH_CASE_HPP
#define SWITCH_CASE_HPP
#include <initializer_list>
#include <vector>
namespace lee {

template<class T>
class Case {
public:
	Case(T t):Type(t),Types(){}
	Case(T t, std::function<void()> cb):Type(t), Types(),Callback(cb) { }
	Case(std::initializer_list<T> list, std::function<void()> cb) :
		Type(),Types(list), Callback(cb)
	{}
	bool checkAndCall(T t) {
		if (!Callback) { return false; }
		if (Types.size() <= 0) {
			if (t != Type) { return false; }
			Callback();
			return true;
		}
		for (auto& ref : Types) {
			if (ref != t) { continue; }
			Callback();
			return true;
		}
		return false;
	}
private:
	T Type;
	std::vector<T> Types;
	std::function<void()> Callback;
};

template<class T>
class Switch{
public:
	Switch(T t,std::initializer_list<Case<T>> list) {
		T type = t;
		for(Case<T> var : list)
		{
			if (var.checkAndCall(t)) { return; }
		}
	}
};

};
#define SWITCH(type,val) {lee::Switch<type> s(val,{
#define SWITCH_WS(val) {lee::Switch<std::wstring> s(val,{
#define SWITCH_S(val) {lee::Switch<std::string> s(val,{
#define SWITCHEnd });}
#define END });}
#define CASE(value) {value,[&]()
#define CASES(value,...) {{value,##__VA_ARGS__},[&]()
#define CASEEnd },
#define BREAK },
#endif // !SWITCH_CASE_HPP
/*
SWITCH(std::string, "hello")
CASE("hello") {
	///TODO
}BREAK
CASE("test") {
	///TODO
}BREAK
End
*/