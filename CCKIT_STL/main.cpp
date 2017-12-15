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
#include "CCKIT/deque.h"
#include "CCKIT/priority_queue.h"
#include "CCKIT/algorithm.h"
#include "CCKIT/experimental/graph.h"
#include "CCKIT/experimental/csv_map.h"
#include "CCKIT/experimental/maze_gen.h"
#include "CCKIT/math/matrix.h"
#include "CCKIT/spatial partitioning/quadtree.h"
#include <list>
#include <string>
#include <functional>
#include <stack>
#include <queue>

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

	/*foo(const foo& _other)
		: mV0(_other.mV0)
		, mV1(_other.mV1)
	{}

	foo(foo&& _other)
		: mV0(_other.mV0)
		, mV1(cckit::move(_other.mV1))
	{}

	foo& operator=(const foo& _rhs)
	{
		foo temp = _rhs;
		cckit::swap(*this, temp);
		return *this;
	}*/
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

	//vector0.reserve(4);
	//vector0.reserve(8);
	//vector2.reserve(16);

	vector0.insert(vector0.cbegin(), { foo(1, "1th"), foo(2, "2th"), foo(3, "3th"), foo(4, "4th") });
	vector0.insert(vector0.cbegin(), { foo(-2, "-2th"), foo(-1, "-1th"), foo(0, "0th") });
	vector0.insert(vector0.end(), 4, foo(50, "50"));
	vector0.emplace_back(51, "51th");
	vector0.pop_back(); vector0.pop_back();
	vector0.resize(25);
	vector0.resize(20);

	vector1.swap(vector0);
	vector1.shrink_to_fit();
	vector1.push_back(foo(100, "100"));
	vector0 = vector1;
	vector0 = cckit::move(vector1);
	vector1 = cckit::move(vector2);
	vector2 = { foo(), foo() };
	auto iter0 = vector0.erase(vector0.find(2), vector0.find(7));
	for (auto end = vector0.end(); iter0 != end; ++iter0)
		cout << *iter0 << ", ";
	cout << endl << endl;

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
	cckit::map<int, std::string> tree1;
	//cckit::multimap<int, std::string> tree1;
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

	cckit::vector<cckit::pair<const int, std::string> > list0 = { cckit::make_pair(17, "17th"), cckit::make_pair(27, "27th") };
	//cckit::pair<const int, std::string> array0[] = { cckit::make_pair(17, "17th"), cckit::make_pair(27, "27th") };
	tree1.insert(list0.begin(), list0.end());
	
	cout << "erase(4) = " << tree1.erase(4) << endl;
	cout << "erase(13) = " << tree1.erase(13) << endl;
	cout << "erase(1) = " << tree1.erase(1) << endl;
	
	tree1.erase(tree1.find(3), tree1.find(20));
	tree1.try_emplace(10, "10th");
	tree1.try_emplace(11, "11th");
	tree1.insert_or_assign(10, "15th");
	tree1.insert_or_assign(12, "15th");
	tree1[10] = "10";
	tree1[13] = "13";
	try {
		tree1.at(10) = "11";
		tree1.at(14) = "11";
	}
	catch (const std::out_of_range& _exception) {
		cout << _exception.what() << endl;
	}
	
	cout << "size = " << tree1.size() << endl;

	for (auto current = tree1.begin(), end = tree1.end(); current != end; current++)
		cout << current->first << endl;
	cout << endl;
	
	tree1.preorder_walk([](treetype1::const_iterator _current) {
		cout << "(" << _current->first << ", " << _current->second << ")" << endl;
	}, tree1.root());
	cout << endl;
	tree1.preorder_walk([](treetype1::iterator _current) {
		cout << _current.mpNode->mHeight << endl;
	}, tree1.root());
	cout << endl;
	tree1.inorder_walk([](treetype1::iterator _current) {
		cout << "(" << _current->first << ", " << _current->second << ")" << endl;
	}, tree1.root());
	cout << endl;
	tree1.postorder_walk([](treetype1::iterator _current) {
		cout << "(" << _current->first << ", " << _current->second << ")" << endl;
	}, tree1.root());
}

void test_matrix()
{
	cckit::matrix<3, 3> mat =
	{ { 1, 0, 2 }
	, { 2, 1, 1 }
	, { 0, 12, 1 } };

	auto inverseMat = mat.inverse_gauss_jordan_elimination();
	auto multMat = cckit::multiply(mat, inverseMat);
	for (int i = 0; i < multMat.ROW; ++i, cout << endl)
	for (int j = 0; j < multMat.COL; ++j) {
	multMat[i][j] = ((cckit::abs(multMat[i][j]) < .001) ? 0 : multMat[i][j]);
	cout << multMat[i][j] << " ";
	}
	cout << endl;

	cout << cckit::determinant(mat) << endl;
	cout << cckit::determinant(mat) << endl;
}

void test_deque()
{
	{
		cckit::deque<int> map0;
		cckit::vector<int> vector0 = { 55, 56, 57, 58, 59, 60, 61, 62 };
		for (int i = 0; i < 9; ++i)
			map0.insert(map0.end(), i << 1);
		map0.insert(map0.begin() + 2, 3);
		map0.insert(map0.end() - 1, vector0.cbegin(), vector0.cend());
		//auto insertPos = map0.insert(map0.end() - 1, { 55, 56, 57, 58 });
		//auto insertPos = map0.insert(map0.begin(), 8, 65);
		map0.shrink_to_fit();
		map0.insert(map0.cbegin() + 1, { 55, 56, 57, 58, 59 });

		map0.erase(map0.cbegin()); map0.erase(map0.cbegin()); map0.erase(map0.cbegin());
		map0.erase(map0.cbegin()); map0.erase(map0.cbegin()); map0.erase(map0.cbegin());
		map0.insert(map0.cbegin(), 1);
		map0.erase(map0.cend() - 1); map0.erase(map0.cend() - 1); map0.erase(map0.cend() - 4);
		map0.erase(map0.cbegin() + 3);
		map0.insert(map0.cend(), 62); map0.insert(map0.cend() - 7, 54);
		map0.insert(map0.cbegin(), 0);
		auto erasePos = map0.cbegin();
		erasePos = map0.erase(map0.cbegin() + 3);
		map0.emplace(map0.cend() - 4, 58);

		erasePos = map0.erase(map0.cbegin() + 1, map0.cbegin() + 5);
		decltype(map0) map1 = map0;

		erasePos = map0.cbegin();

		map1.verify_block_distribution([](bool _bFlag) {
			cout << (_bFlag ? "+" : "-") << " ";
		});
		cout << endl;
		for (auto iter = erasePos; iter != map0.cend(); iter = 1 + iter)
			cout << *iter << endl;
		cout << endl;
		struct Impl {
			static void output(cckit::add_lvalue_reference<cckit::add_const<decltype(map0)>::type>::type _map) {
				for (int i = 0, size = _map.size(); i < size; ++i)
					cout << _map[i] << endl;
			}
		};
		Impl::output(map1);
		cout << endl;
		for (auto iter = map0.rbegin(); iter != map0.rend(); iter = 1 + iter)
			cout << *iter << endl;
		cout << endl;
	}
}

void test_quadtree()
{
	cckit::quadtree<int> qt(cckit::quadtree<int>::rect(cckit::quadtree<int>::point(1.0f, 1.0f), cckit::quadtree<int>::point(5.0f, 7.0f)));
	qt.insert(0, decltype(qt)::point(2.0f, 1.0f));
	qt.insert(1, decltype(qt)::point(2.0f, 2.0f));
	qt.insert(2, decltype(qt)::point(4.9f, 2.0f));
	qt.insert(3, decltype(qt)::point(4.5f, 6.0f));
	qt.insert(4, decltype(qt)::point(1.6f, 5.0f));

	qt.insert(5, decltype(qt)::point(4.0f, 1.0f));
	qt.insert(6, decltype(qt)::point(3.5f, 1.1f));
	qt.insert(7, decltype(qt)::point(4.5f, 1.1f));

	qt.insert(10, decltype(qt)::point(1.5f, 5.0f));
	qt.insert(8, decltype(qt)::point(1.5f, 5.1f));
	qt.insert(9, decltype(qt)::point(1.5f, 4.1f));

	cout << qt.size() << endl;

	decltype(qt)::elemlist_type list0;
	qt.query_range(decltype(qt)::rect(decltype(qt)::point(1.0f, 1.0f), decltype(qt)::point(2.01f, 5.001f)), list0);
	cout << list0.size() << endl;

	/*qt.verify([](const decltype(qt)::value_type& _arg) {
	cout << _arg << " ";
	}, []() {
	cout << "nil" << endl;
	}, []() {
	cout << "branch BEGIN" << endl;
	}, []() {
	cout << "branch END" << endl;
	});*/
}

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

	void func() const { cout << "const" << endl; }
	void func() { cout << "non-const" << endl; const_cast<const B*>(this)->func(); }

	void func0() const { cout << "const" << endl; (const_cast<B*>(this))->func0(); }
	void func0() { cout << "non-const" << endl; }

	void func1(int&&) { cout << "move" << endl; }
	void func1(const int&) { cout << "copy" << endl; }

	template<class T>
	void func2(T&& _arg) { func1(cckit::forward<T>(_arg)); }
};

//#define NSP ::std::
#define NSP ::cckit::

std::ostream& operator<<(std::ostream& _os, const NSP pair<int, int>& _arg) 
{
	return _os << "(" << _arg.first << ", " << _arg.second << ")";
}
#include "CCKIT/math/arithmetic.h"
int main()
{	
	int temp;
	
	cout << cckit::mult(cckit::add(15, 23), 25) << endl;

	/*NSP deque<NSP pair<int, int>> vector0
		= { NSP	make_pair(3, 3), NSP make_pair(1, 1), NSP make_pair(13, 13)
		, NSP make_pair(9, 9), NSP make_pair(6, 6), NSP make_pair(12, 12) };
	NSP	priority_queue<NSP pair<int, int> > heap1;
	//cckit::binary_heap<cckit::pair<int, int> > heap1;
	decltype(heap1) heap0(decltype(heap1)::value_compare(), vector0);
	heap0.push(NSP make_pair(3, 3));
	heap0.emplace(1, 1);

	decltype(heap0) heap2 = cckit::move(heap0);
	heap0.swap(heap2);
	cckit::swap(heap0, heap2);
	//heap2.modify(2, 13, 14);	

	cout << "heap0" << endl;
	for (; !heap0.empty(); cout << heap0.top() << " ", heap0.pop()) {}
	cout << endl;
	cout << "heap2" << endl;
	for (; !heap2.empty(); cout << heap2.top() << " ", heap2.pop()) {}
	cout << endl;*/

	/*cout << "heap0" << endl;
	for (decltype(heap0)::size_type i = 0; i < heap0.size(); ++i)
		cout << heap0[i] << " ";
	cout << endl << "is_heap = " << heap0.validate() << endl;
	cout << "heap2" << endl;
	for (decltype(heap2)::size_type i = 0; i < heap2.size(); ++i)
		cout << heap2[i] << " ";
	cout << endl << "is_heap = " << heap2.validate() << endl;*/
	
	//test_deque();
	//test_setmap();
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