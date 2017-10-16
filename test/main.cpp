#include "get_set.hpp"
#include "switch_case.hpp"

#include <iostream>
#include <string>

struct TestType {
	int t1;
	int t2;
};
using namespace lee;
class Test {
	int m_id;
	std::string m_name;
	TestType m_t;
public:
	GetSet<int> ID;
	GetSet<std::string> Name;
	GetSet<TestType> TValue;
	Test() :
		ID( GetVal{ 
				return m_id; 
			},
			SetVal(int) {
				m_id = value; 
			}),
		Name(GetVal{
				return m_name;
			},
			SetVal(std::string) {
				m_name = value;
			}),
		TValue(GetVal{
				return m_t;
			},
			SetVal(TestType) {
				m_t = value;
			})
	{ }
};


int main(){
	
	Test t;
	t.ID = 100;
	int id = t.ID;
	t.Name = "lee";
	//std::string name = ;
	t.TValue = { 100,200 };
	std::cout << t.ID << t.Name.toString() << TestType(t.TValue).t1 << std::endl;
	std::string hello("hello");
	Switch<int> s(10,{ { 100,[]() {} }, });

	SWITCH(std::string, "hello")
		CASE("hello") {
			t.ID = 200;
			t.Name = "hello";
		}BREAK
		CASE("test") {
			t.ID = 300;
			t.Name = "test";
		}BREAK
	END;

	SWITCH(int,t.ID)
		CASE(100) {
			t.ID = 200;
		}BREAK
		CASE(200) {
			t.Name = "test";
		}BREAK
	END
	return 0;
}