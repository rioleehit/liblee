#include "get_set.hpp"
#include "switch_case.hpp"

#include <iostream>
#include <string>
#include <assert.h>

struct TestType {
	int t1;
	int t2;
};
class FunCallCount{
public:
	FunCallCount(){
		std::cout<<"creat at "<<(long)this%10000<<std::endl;
	}
	FunCallCount(int c):m_count(c){
		std::cout<<"creat c at "<<(long)this%10000<<std::endl;
	}
	FunCallCount(const FunCallCount& f){
		m_count = f.m_count;
		std::cout<<"copy  c at "<<(long)this%10000 <<   " from " << (long)&f%10000 << std::endl;
	}
	~FunCallCount(){
		std::cout<<"destroy "<<(long)this%10000<<std::endl;
	}

private:
	int m_count;
};
using namespace lee;
class Test {
	int m_id;
	std::string m_name;
	TestType m_t;

	int m_testI;
	FunCallCount m_testFCallCount;
public:
	GetSet<int> ID;
	GetSet<std::string> Name;
	GetSet<TestType> TValue;
	GetSet<int> TEST_I;
	GetSet<FunCallCount> CALL_COUNT;
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
			}),
		TEST_I(GetVal{ return m_testI;},
			SetVal(int){
				m_testI = value;
			}),
		CALL_COUNT(GetRef(FunCallCount){return m_testFCallCount;},
			SetVal(FunCallCount){m_testFCallCount = value;})
		,m_testFCallCount()

	{ }

	void test(){
		std::cout<<"uninit m_testI\t" << TEST_I << std::endl;
		TEST_I = 1;
		//assert(m_testI == 1);
		std::cout<<"m_testI == " << TEST_I << std::endl;
		const int& m_i_ref = TEST_I;
		//*m_i_ref = 100;
		std::cout<<"m_i_ref == " << TEST_I << std::endl
		<<"&TEST_I\t" << &m_i_ref << std::endl
		<<"m_testI\t" << &m_testI << std::endl;
	}
	void testCallCount(){
		{
			const FunCallCount& f = CALL_COUNT;

		}
	std::cout << std::endl;
		{
			CALL_COUNT = FunCallCount(1);
		}
		
	}
};


int main(){
	
	Test t;
	t.test();
	std::cout << std::endl;
	t.testCallCount();

	std::cout << std::endl;

/*
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
	*/
	return 0;
}