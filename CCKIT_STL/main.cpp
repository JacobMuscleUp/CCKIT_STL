#include "CCKIT/utility.h"
#include "CCKIT/functional.h"
#include "CCKIT/allocator.h"
#include "CCKIT/list.h"
#include "CCKIT/vector.h"
#include "CCKIT/functional.h"
#include "CCKIT/random.h"
#include "CCKIT/memory.h"
#include "CCKIT/stack.h"
#include "CCKIT/queue.h"
#include "CCKIT/set.h"
#include "CCKIT/map.h"
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

struct foo
{
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
	return _os << "(" << _a.mV0 << ", " << _a.mV1.c_str() << ")";
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
	/*const auto arrSize = cckit::array_size(a);
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
		cout << i << endl;*/

	const auto aSize = cckit::array_size(a);
	auto head = cckit::adjacent_find(a, a + aSize, [](decltype(*a) _arg0, decltype(*a) _arg1) { return 33.0/6 * _arg0 ==  _arg1; });
	cckit::for_each(head, a + aSize, [](const decltype(*head)& _arg) {
		cout << _arg << endl;
	});
}

void test_graph()
{
	cckit::csv_map csvmap0("../input.csv");
	if (csvmap0.initialized)
		csvmap0.search(2, 2, 5);
	csvmap0.write("../output.csv");
}

void test_vector()
{
	typedef cckit::vector<foo> vectortype0;
	vectortype0 vector0{ foo(5, "5th"), foo(4, "4th"), foo(15, "15th") };
	vectortype0 vector1 = vector0, vector2(cckit::move(vector1));

	vector0.reserve(4);
	vector2.reserve(16);

	vector0.insert(vector0.cend(), foo(10, "10th"));
	vector0.emplace_back(11, "11th");
	vector0.push_back(foo(12, "12th"));
	vector0.pop_back();
	vector0.insert(vector0.cbegin(), { foo(1, "1th"), foo(2, "2th"), foo(3, "3th"), foo(4, "4th") });
	vector0.insert(vector0.cbegin(), { foo(-2, "-2th"), foo(-1, "-1th"), foo(0, "0th") });

	vector1.swap(vector0);
	vector1.shrink_to_fit();
	vector1.pop_back();

	vector0.insert(vector0.cbegin(), { foo(1, "1th"), foo(2, "2th"), foo(3, "3th"), foo(4, "4th") });
	vector0.clear();
	vector0.insert(vector0.cbegin(), { foo(1, "1th"), foo(2, "2th"), foo(3, "3th") });
	vector0.resize(5);
	vector0.resize(4);
	vector0.resize(8);

	cout << "vector0" << endl;
	for (auto elem : vector0)
		cout << elem << ", ";
	cout << vector0.capacity() << endl;
	cout << "vector1" << endl;
	for (auto elem : vector1)
		cout << elem << ", ";
	cout << vector1.capacity() << endl;
	cout << "vector2" << endl;
	for (auto elem : vector2)
		cout << elem << ", ";
	cout << vector2.capacity() << endl;
	/*cckit::vector<int> vector0 { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	vector0 = { 3, 1, 65, 2 };
	decltype(vector0) vector2(cckit::move(vector0)), vector1;
	vector1 = vector2;
	vector1.insert(vector1.find(2), 5, 7);
	vector1.emplace(vector1.find(1), 66);
	cout << "vector1.capacity() = " << vector1.capacity() << endl;

	for (auto &i : vector1) 
		cout << i << " ";
	cout << '\n';
	for (decltype(vector1)::size_type i = 0; i < vector1.size(); ++i)
		cout << vector1[i] << " ";
	cout << endl;
	for (decltype(vector1)::reverse_iterator current = vector1.rbegin(), head = current; current != vector1.rend(); ++current)
		cout << head[vector1.size() - 1 - cckit::distance(vector1.rbegin(), current)] << " ";
	cout << endl;*/
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

void test_setmap()
{
	cout << "SET" << endl;
	cckit::multiset<int> tree0;
	//cckit::set<int> tree0;
	typedef decltype(tree0) treetype;

	tree0.insert(4);
	tree0.insert(1);
	tree0.insert(1);
	tree0.insert(7);
	tree0.insert(2);
	tree0.insert(2);
	tree0.insert(2);
	tree0.insert(3);

	tree0.preorder_walk([](treetype::iterator _current) {
		cout << *_current << endl;
	}, tree0.root());
	cout << endl;
	tree0.inorder_walk([](treetype::iterator _current) {
		cout << *_current << endl;
	}, tree0.root());
	cout << endl;
	tree0.postorder_walk([](treetype::iterator _current) {
		cout << *_current << endl;
	}, tree0.root());

	cout << endl;
	cout << "MAP" << endl;
	//cckit::map<int, std::string> tree1;
	cckit::multimap<int, std::string> tree1;
	typedef decltype(tree1) treetype1;

	tree1.insert(cckit::make_pair(4, "4th"));
	tree1.insert(cckit::make_pair(4, "4th"));
	tree1.insert(cckit::make_pair(1, "1st"));
	tree1.insert(cckit::make_pair(7, "7th"));
	tree1.insert(cckit::make_pair(2, "2nd"));
	tree1.insert(cckit::make_pair(3, "3rd"));
	tree1.insert(cckit::make_pair(3, "33rd"));
	tree1.emplace(6, "6th");
	tree1.emplace(5, "6th");
	tree1.insert(--tree1.end(), cckit::make_pair(1, "3rd"));
	tree1.insert(tree1.begin(), cckit::make_pair(20, "20th"));
	tree1.insert({ cckit::make_pair(13, "13th"), cckit::make_pair(23, "23rd"), cckit::make_pair(33, "33rd") });

	cckit::list<cckit::pair<const int, std::string> > list0 = { cckit::make_pair(17, "17th"), cckit::make_pair(27, "27th") };
	//cckit::pair<const int, std::string> array0[] = { cckit::make_pair(17, "17th"), cckit::make_pair(27, "27th") };
	tree1.insert(list0.begin(), list0.end());
	
	/*for (auto current = tree1.begin(); tree1.size() > 0; current = tree1.erase(current)) {}
	tree1.insert(cckit::make_pair(7, "7th"));
	tree1.insert(cckit::make_pair(2, "2nd"));
	tree1.insert(cckit::make_pair(3, "3rd"));
	tree1.insert(cckit::make_pair(3, "3rd"));*/
	cout << "erase(4) = " << tree1.erase(4) << endl;
	cout << "erase(13) = " << tree1.erase(13) << endl;
	cout << "erase(1) = " << tree1.erase(1) << endl;
	
	tree1.erase(tree1.find(3), tree1.find(20));
	decltype(tree1) tree2; 
	tree1.swap(tree2);
	cout << "size = " << tree1.size() << endl;
	
	tree1.preorder_walk([](treetype1::iterator _current) {
		cout << "(" << _current->first << ", " << _current->second << ")" << endl;
	}, tree1.root());
	cout << endl;
	/*tree1.preorder_walk([](treetype1::iterator _current) {
		cout << _current.mpNode->mHeight << endl;
	}, tree1.root());
	cout << endl;*/
	tree1.inorder_walk([](treetype1::iterator _current) {
		cout << "(" << _current->first << ", " << _current->second << ")" << endl;
	}, tree1.root());
	cout << endl;
	tree1.postorder_walk([](treetype1::iterator _current) {
		cout << "(" << _current->first << ", " << _current->second << ")" << endl;
	}, tree1.root());
}

/*void* operator new[](std::size_t _bytes, const char*)
{
	cout << "bytes = " << _bytes << endl;
	return ::operator new[](_bytes);
}
struct Test
{
	int* mA;
	Test(int a) { mA = new int(a); }
	~Test() { delete mA; }
};*/
class A
{
public:
	int mV;

	A() { cout << "A def ctor" << endl; }
	A(const A&) { cout << "A copy ctor" << endl; }
	A(A&&) { cout << "A move ctor" << endl; }
	A& operator=(const A&) { cout << "A copy =" << endl; }
};

class B 
{
public:
	A mA;

	//B() { cout << "B ctor" << endl; }
	explicit B(A _a = A()) : mA(_a) { cout << "B ctor" << endl; }
};

int main()
{	
	
	int temp;
	
	/*cckit::multiset<int> tree0;
	//cckit::set<int> tree0;
	typedef decltype(tree0) treetype;

	tree0.insert(4);
	tree0.insert(1);
	tree0.insert(1);
	auto pair1 = tree0.insert(7);
	tree0.insert(2);
	tree0.insert(2);
	auto pair0 = tree0.insert(2);
	tree0.insert(3);
	tree0.emplace(5);

	for (auto current = tree0.find(4); current != tree0.cend(); current++)
		cout << *current << endl;
	cout << endl;
	for (auto current = tree0.rbegin(); current != tree0.rend(); ++current)
		cout << *current << endl;
	cout << endl;
	for (auto current = ----tree0.begin(); current != tree0.end(); current--)
		cout << *current << endl;*/
	
	test_setmap();
	//test_vector();
	//test_graph();
	//test_algorithm();
	//test_list();
	//test_mazegen();
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