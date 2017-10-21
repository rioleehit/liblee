#ifndef SWITCH_CASE_HPP
#define SWITCH_CASE_HPP
#include <initializer_list>
namespace lee {

template<class T>
class Case {
public:
	Case(T t):Type(t){}
	Case(T t, std::function<void()> cb):Type(t),Callback(cb) { }
	T Type;
	std::function<void()> Callback;
};

template<class T>
class Switch{
public:
	Switch(T t,std::initializer_list<Case<T>> list) {
		T type = t;
		for(Case<T> var : list)
		{
			if (type != var.Type || !var.Callback) continue;
			var.Callback();
		}
	}
};

};
#define SWITCH(type,val) {lee::Switch<type> s(val,{
#define SWITCHEnd });}
#define END });}
#define CASE(value) {value,[&]()
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