#include "CCKIT/utility.h"
#include "CCKIT/functional.h"
#include "CCKIT/allocator.h"
#include "CCKIT/list.h"
#include "CCKIT/functional.h"
#include "CCKIT/random.h"
#include "CCKIT/memory.h"
#include "CCKIT/stack.h"
#include "CCKIT/queue.h"
#include "CCKIT/algorithm.h"
#include "CCKIT/experimental/graph.h"
#include "CCKIT/experimental/csv_map.h"
#include "CCKIT/experimental/maze_gen.h"
#include <list>
#include <string>
#include <functional>
#include <stack>

#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>

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

/// cannot use std::string since cckit::list currently fails to handle the deallocation related to placement new
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
	cckit::list<foo, cckit::allocator_malloc> list;
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
		for (auto iter = list1.cbegin(); iter != list1.cend(); ++iter)
			cout << *iter << endl;
	}
}

void test_stack()
{
	cckit::stack<foo> stack0;

	const size_t numChoices = cckit::array_size(nums);
	for (size_t i = numChoices; i > 0; --i)
		nums[i - 1] = i;
	for (size_t i = 0; i < listSize; ++i) {
		fooList[i] = foo(nums[i], strs[i]);
	}

	for (auto i = listSize; i > 0; --i)
		stack0.push(cckit::move(fooList[i - 1]));
	for (decltype(stack0)::size_type i = 0; i < stack0.size(); ++i)
		cout << stack0[i] << endl;
	cout << endl;
	while (!stack0.empty()) {
		cout << stack0.top() << endl;
		stack0.pop();
	}
}

void test_queue()
{
	cckit::queue<foo> queue0;

	const size_t numChoices = cckit::array_size(nums);
	for (size_t i = numChoices; i > 0; --i)
		nums[i - 1] = i;
	for (size_t i = 0; i < listSize; ++i) {
		fooList[i] = foo(nums[i], strs[i]);
	}

	for (auto i = listSize; i > 0; --i)
		queue0.push(cckit::move(fooList[i - 1]));
	for (decltype(queue0)::size_type i = 0; i < queue0.size(); ++i)
		cout << queue0[i] << endl;
	cout << endl;
	while (!queue0.empty()) {
		cout << queue0.front() << endl;
		queue0.pop();
	}
}

void test_algorithm()
{
	cckit::list<int> v = { 3, 7, 1, 5, 2, 2, 2, 7, 7, 4 };
	int a[] = { 3, 7, 1, 5, 2, 2, 2, 7, 7, 4, 6, 33 };
	int b[] = { 8, 2 };
	int c[] = { 2, 5, 3 };
	const auto arrSize = cckit::array_size(a);
	for (size_t i = 0; i < arrSize; ++i) {
		cout << a[i] << endl;
	}
	cout << endl;

	cckit::selection_sort(v.begin(), v.end());
	//cckit::insertion_sort(a, a + arrSize);
	cckit::Heapsort(a);
	for (size_t i = 0; i < arrSize; ++i) {
		cout << a[i] << endl;
	}
	cout << "min = " << *cckit::min_element(a, a + cckit::array_size(a)) << endl;
	cout << "max = " << *cckit::max_element(a, a + cckit::array_size(a)) << endl;
	cout << endl;

	for (auto i : v)
		cout << i << endl;
}

void test_graph()
{
	cckit::csv_map csvmap0("../input.csv");
	if (csvmap0.initialized)
		csvmap0.search(2, 2, 5);
	csvmap0.write("../output.csv");
}

void test_mazegen()
{
	cckit::grid_dist maze(12, 12);
	//huntnkill_mazegen(maze);
	rec_backtrack_mazegen(maze);
	//rec_backtrack_mazegen_2(maze, maze.rand_cell());
	//bintree_mazegen(maze);
	maze.update_dists(maze.at(3, 3));
	maze.calc_path(maze.at(8, 7));
	cout << maze << endl;
	for (int i = 0; i < 5; ++i) { cout << endl; }
	maze.update_dists(maze.at(3, 3));
	maze.calc_path(maze.at(0, 0));
	cout << maze << endl;
}

int main()
{	
	int temp;

	//test_graph();
	//test_algorithm();
	//test_list();
	test_mazegen();
	//test_stack();
	//test_queue();
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