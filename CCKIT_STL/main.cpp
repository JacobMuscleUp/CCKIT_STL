#include "CCKIT/utility.h"
#include "CCKIT/functional.h"
#include "CCKIT/allocator.h"
#include "CCKIT/list.h"
#include "CCKIT/functional.h"
#include "CCKIT/random.h"
#include <list>
#include <string>

#include <iostream>
using std::cout; using std::endl;

/*struct foo
{
	int mV0;
	const char* mV1;

	foo()
		: mV0(3), mV1("default")
	{}

	foo(int _v0, const char* _v1)
		: mV0(_v0), mV1(_v1)
	{}
};

inline std::ostream& operator<<(std::ostream& _os, const foo& _a) noexcept
{
	return _os << "(" << _a.mV0 << ", " << _a.mV1 << ")" << endl;
}*/

class foo
{
public:
	int mV0;
	std::string mV1;

	foo()
		: mV0(3), mV1("default")
	{}

	foo(int _v0, const std::string& _v1)
		: mV0(_v0), mV1(_v1)
	{}
};

inline std::ostream& operator<<(std::ostream& _os, const foo& _a) noexcept
{
	return _os << "(" << _a.mV0 << ", " << _a.mV1.c_str() << ")" << endl;
}

inline bool operator==(const foo& _arg0, const foo& _arg1) noexcept
{
	return _arg0.mV0 == _arg1.mV0;
}
inline bool operator!=(const foo& _arg0, const foo& _arg1) noexcept
{
	return _arg0.mV0 != _arg1.mV0;
}

namespace cckit
{
	template <>
	struct less<foo>
	{
		CCKIT_CONSTEXPR bool operator()(const foo& _arg0, const foo& _arg1) const
		{
			return _arg0.mV0 < _arg1.mV0;
		}
	};
	template <>
	struct greater<foo>
	{
		CCKIT_CONSTEXPR bool operator()(const foo& _arg0, const foo& _arg1) const
		{
			return _arg1.mV0 < _arg0.mV0;
		}
	};
}// namespace cckit

const char* strs[] = {"1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th" };
int nums[cckit::array_size(strs)];
const size_t listSize = 10;
foo fooList[listSize];

void test_list()
{
	//std::list<foo> list;
	cckit::list<foo> list;
	typedef decltype(list) list_type;
	{
		const size_t numChoices = cckit::array_size(nums);
		for (size_t i = numChoices; i > 0; --i)
			nums[i - 1] = i;
		for (size_t i = 0; i < listSize; ++i) {
			size_t rand = cckit::rand_int(0, numChoices);
			fooList[i] = foo(nums[rand], strs[rand]);
		}

		list_type list0 = { foo(99, "99th"), foo(-11, "-11th") };
		for (size_t i = 0; i < listSize; ++i) {
			list0.push_back(fooList[i]);
			list0.emplace_back(fooList[i].mV0, fooList[i].mV1);
		}

		cout << "list0.size() = " << list0.size() << endl;
		cout << "INITIAL" << endl;
		for (auto iter = list0.cbegin(); iter != list0.cend(); ++iter)
			cout << *iter << endl;
		cout << "AFTER SORT" << endl;

		list0.sort(cckit::conditional_t<
			cckit::is_same<list_type, cckit::list<list_type::value_type> >::value
			, cckit::greater<list_type::value_type>
			, cckit::less<list_type::value_type>
		>());

		for (auto iter = list0.cbegin(); iter != list0.cend(); ++iter)
			cout << *iter << endl;

		cout << "AFTER UNIQUE" << endl;
		list0.unique();
		for (auto iter = list0.cbegin(); iter != list0.cend(); ++iter)
			cout << *iter << endl;

		cout << "AFTER REMOVE 4" << endl;
		list0.remove(foo(4, "7th"));
		/*list0.remove_if([](list_type::const_reference _val) {
			return _val.mV0 % 2 == 1;
		});*/
		for (auto iter = list0.cbegin(); iter != list0.cend(); ++iter)
			cout << *iter << endl;

		cout << "AFTER REVERSE" << endl;
		list0.reverse();
		for (auto iter = list0.cbegin(); iter != list0.cend(); ++iter)
			cout << *iter << endl;

		list_type list1 = cckit::move(list0);
		cout << "AFTER MOVING list0" << endl;

		cout << "list0.size() = " << list0.size() << endl;
		for (auto iter = list0.cbegin(); iter != list0.cend(); ++iter)
			cout << *iter << endl;
		cout << endl;
		cout << "list1.size() = " << list1.size() << endl;
		for (auto iter = list1.rbegin(); iter != list1.crend(); iter++)
			cout << iter->mV0 << ", " << iter->mV1 << endl;
	}
}

int main()
{	
	int temp;
	test_list();
	std::cin >> temp;
	return 1;
}


/*void(*pFunc0)();

template<typename Tret, typename T>
Tret RunLambdaPtr(T* v) {
	return (Tret)(*v)();
}
template<typename Tret = void, typename Tfp = Tret(*)(), typename T>
Tfp LambdaPrt(T&) {
	return (Tfp)RunLambdaPtr<Tret, T>;
}

void EXPERIMENTAL()
{
	auto pLambda0 = [list2]() {
		cout << endl;
		cout << "list2::operator[]" << endl;
		for (list_type::size_type i = 0; i < list2.size(); ++i)
			cout << list2[i] << endl;
	};
	pFunc0 = LambdaPrt(pLambda0);
}*/