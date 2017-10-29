#include "get_set.hpp"
#include "switch_case.hpp"
#include "parser.hpp"
#include "util.hpp"

#include <iostream>
#include <string>
#include <assert.h>

struct TestType {
	int t1;
	int t2;
};
class FunCallCount{
public:
	FunCallCount(const FunCallCount& f){
		m_count = f.m_count;
		
	}
	FunCallCount(FunCallCount& f) {
		m_count = f.m_count + 1;
		outStr += m_count;
		std::cout << "copy  c at " << (long)this % 10000 << " from " << (long)&f % 10000 << std::endl;
	}
	//FunCallCount(FunCallCount&& f){
	//	m_count = f.m_count + 1;
	//	outStr += m_count;
	//	std::cout << "copy FunCallCount c at " << (long)this % 10000 << " from " << (long)&f % 10000 << std::endl;
	//}
	FunCallCount(){
		outStr = "FunCallCount";
		std::cout<<"creat at "<<(long)this%10000<<std::endl;
	}
	//FunCallCount(int c):m_count(c){
	//	std::cout<<"creat c at "<<(long)this%10000<<std::endl;
	//}
	~FunCallCount(){
		std::cout<<"destroy "<<(long)this%10000<<std::endl;
	}
	void operator =(const FunCallCount& f) {
		m_count = f.m_count + 1;
		outStr += m_count;
		std::cout << "copy operator c at " << (long)this % 10000 << " from " << (long)&f % 10000 << std::endl;
	}
	operator std::string&() {
		return outStr;
	}
	int m_count;
private:
	std::string outStr;
};
using namespace lee;

template<class T>
class TestSet {
public:
	Set<T> mVAL;
	TestSet() : mVAL(SetRef(T){
		m_val = value;
	})
	{ }
	T m_val;
private:
};
#define MyTestSet(type,val) { \
	TestSet<type> t_set; \
	t_set.mVAL = (val);	\
	std::cout << t_set.m_val << std::endl;\
}


template<class T>
class TestGet {
public:
	Get<T> mVAL;
	TestGet() : mVAL(GetVal{
		std::cout << "this " << (void*)this << " mval: " << &m_val << std::endl;
		return m_val;
	})
	{ }
	T m_val;
private:
};
#define MyTestGet(type,val) { \
TestGet<type> t_get; \
t_get.m_val = (val); \
type&& t= t_get.mVAL; \
std::cout << &t << "\t"<< t <<std::endl; \
}

template<class T>
class TestGetSet {
public:
	GetSet<T> mVAL;
	TestGetSet() : 
		mVAL(GetVal{
			std::cout << "get this " << (void*)this << " &mval: " << &m_val << std::endl;
			return m_val;
		}, SetRef(T){ 
			std::cout << "set this " << (void*)this << " mval: " << m_val << " val: " << value << " &val: " << &value << std::endl;
			m_val = value; 
		})
	{ }
	T m_val;
private:
};
#define MyTestGetSet(type,val) \
{ \
	type v = val;\
	TestGetSet<type> t_get; \
	std::cout << "set val" << std::endl;\
	t_get.mVAL = val; \
	std::cout << "set v" << &v <<std::endl;\
	t_get.mVAL = v; \
	std::cout << "get " <<std::endl;\
	type&& t= t_get.mVAL; \
	std::cout << "&mVAL: "<< (void*)&t << " t: "<< t  <<std::endl; \
}

template<class T>
class TestRefGet {
public:
	RefGet<T> mVAL;
	TestRefGet() : mVAL( GetRef(T){
		std::cout << "this " << (void*)this << " mval: " << &m_val ;
		return m_val;
	})
	{ }
	T m_val;
};
#define MyTestRefGet(type,val) { \
TestRefGet<type> t_get; \
type& ref = t_get.mVAL;	\
std::cout << " ref: "<< &ref << std::endl; \
}

template<class T>
class TestRefGetSet {
public:
	RefGetSet<T> mVAL;
	TestRefGetSet() :
		mVAL(GetRef(T){
			std::cout << "get this " << (void*)this << " &mval: " << &m_val << std::endl;
			return m_val;
		}, SetRef(T){
			std::cout << "set this " << (void*)this << " mval: " << m_val << " val: " << value << " &val: " << &value << std::endl;
			m_val = value;
		})
	{ }
	T m_val;
private:
};
#define MyTestRefGetSet(type,val) \
{ \
	type v = val;\
	TestRefGetSet<type> t_get; \
	std::cout << "set val" << std::endl;\
	t_get.mVAL = val; \
	std::cout << "set v" << &v <<std::endl;\
	t_get.mVAL = v; \
	std::cout << "get " <<std::endl;\
	type& t= t_get.mVAL; \
	std::cout << "&mVAL: "<< (void*)&t << " t: "<< t  <<std::endl; \
}


#include <vector>
#include <initializer_list>
class Test {

	//int m_id;
	//std::string m_name;
	//TestType m_t;

	int m_id;
	FunCallCount m_testFCallCount;
	std::vector<int> m_IDs;
public:
	
	//GetSet<int> ID;
	//GetSet<std::string> Name;
	//GetSet<TestType> TValue;
	//GetSet<int> TEST_I;
	RefGetSet<std::vector<int>> IDs;
	RefGet<int> getRefID;
	Set<FunCallCount> setCALL_COUNT;
	Get<FunCallCount> getCALL_COUNT;
	//RefGet<FunCallCount> getRefCALL_COUNT;
	Test():
		IDs(GetRef(std::vector<int>) {
				return m_IDs;
			},
			SetRef(std::vector<int>){
				m_IDs = value;
			}),
		//m_testFCallCount(),
		setCALL_COUNT(
			SetRef(FunCallCount) {
					this->m_testFCallCount = value;
			}),
		getCALL_COUNT(GetVal{
				return this->m_testFCallCount;
			}),
		getRefID(GetRef(int){return this->m_id; })
		//, getRefCALL_COUNT(
		//	GetRef(FunCallCount) {
		//		return m_testFCallCount;
		//	}
		//)
	{ }

	//void test(){
	//	Name = "test string";
	//	std::string name = Name;
	//	std::cout << name << std::endl;


	//	std::cout<<"uninit m_testI\t" << TEST_I << std::endl;
	//	TEST_I = 1;
	//	//assert(m_testI == 1);
	//	std::cout<<"m_testI == " << TEST_I << std::endl;
	//	const int& m_i_ref = TEST_I;
	//	// *m_i_ref = 100;
	//	std::cout<<"m_i_ref == " << TEST_I << std::endl
	//	<<"&TEST_I\t" << &m_i_ref << std::endl
	//	<<"m_testI\t" << &m_testI << std::endl;
	//}
	//void testCallCount(){
	//	{
	//		auto& f = (FunCallCount&)CALL_COUNT.$();
	//		f.m_count = 100;
	//		
	//		std::cout << CALL_COUNT.$().m_count  << std::endl;
	//	}
	//std::cout << std::endl;
	//	{
	//		CALL_COUNT = FunCallCount(1000);
	//	}
	//	
	//}
};

#include <fstream>
#include <sstream>
int main(){
	MyTestSet(short, 10);
	MyTestSet(int, 10);
	MyTestSet(float, 10.0);
	MyTestSet(double, 10);
	MyTestSet(bool, true);
	MyTestSet(std::string, "hello");

	MyTestGet(short, 10);
	MyTestGet(int, 10);
	MyTestGet(float, 10.0);
	MyTestGet(double, 10);
	MyTestGet(bool, true);
	MyTestGet(std::string, "hello");

	MyTestGetSet(short, 10);
	MyTestGetSet(int, 10);
	MyTestGetSet(float, 10.0);
	MyTestGetSet(double, 10);
	MyTestGetSet(bool, true);
	MyTestGetSet(std::string, "hello");

	MyTestRefGet(short, 10);
	MyTestRefGet(int, 10);
	MyTestRefGet(float, 10.0);
	MyTestRefGet(double, 10);
	MyTestRefGet(bool, true);
	MyTestRefGet(std::string, "hello");

	MyTestRefGetSet(short, 10);
	MyTestRefGetSet(int, 10);
	MyTestRefGetSet(float, 10.0);
	MyTestRefGetSet(double, 10);
	MyTestRefGetSet(bool, true);
	MyTestRefGetSet(std::string, "hello");

	FunCallCount fc;
	fc.m_count = 1000;
	std::cout << "create fc" << std::endl;
	Test t;
	t.setCALL_COUNT = fc;
	t.getRefID.ref();
	//{ TestSet<FunCallCount> t_set; t_set.mVAL = (fc); std::cout << (std::string)t_set.m_val << t_set.m_val.m_count << std::endl; }
	//MyTestSet(FunCallCount, fc);

	t.IDs = { 1,2,3,4,5 };
	auto refVec = t.IDs.ref();
	

	std::ifstream is("data.xml", std::ifstream::binary);
	if (is) {
		// get length of file:
		is.seekg(0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);

		std::vector<char> buff(length + 1);
		char * buffer = &buff[0];

		std::cout << "Reading " << length << " characters... ";
		// read data as a block:
		is.read(buffer, length);

		if (is){
			//lee::for_each_tag(std::string(buffer),);
			lee::DataNode n;

			std::wstring result = lee::to_wchar_t(std::string(buffer));

			n.parse(lee::CvtString((buffer)));
			std::cout << "all characters read successfully.";
		}
		else
			std::cout << "error: only " << is.gcount() << " could be read";
		is.close();

		// ...buffer contains the entire file...
	}

	//Test t;
	////t.test();
	//std::cout << std::endl;
	////t.testCallCount();

	//std::cout << std::endl;

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
