#ifndef HANDLE_HPP
#define HANDLE_HPP
//#include <string>
#include <functional>
#include <list>
namespace lee{
	
template<class T,class PARAM>
	
class HandleBase {
public:

	void operator+=(std::function<void(T*,PARAM*)> hd){
		m_cbs.push_back(hd);
	}
	void call(T*,PARAM*){

	}
private:
	std::list<std::function<void(T*,PARAM*)>> m_cbs;
};
#endif //HANDLE_HPP