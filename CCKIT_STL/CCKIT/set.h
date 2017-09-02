#ifndef CCKIT_SET_H
#define CCKIT_SET_H

#include "internal/avltree.h"
#include "allocator.h"

namespace cckit
{
	template<typename Key, typename Compare = less<Key>, typename Allocator = allocator
		, typename DataStructure = avltree<Key, Key, Compare, Allocator, use_self<Key>, false, true> >
	class set : public DataStructure
	{
	private:
		typedef set<Key, Compare, Allocator, DataStructure> this_type;
		typedef DataStructure base_type;
	public:
		typedef typename base_type::key_type key_type;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::size_type size_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::key_compare key_compare;
		typedef typename base_type::key_compare value_compare;
		typedef typename base_type::allocator_type allocator_type;
		typedef typename base_type::reference reference;
		typedef typename base_type::const_reference const_reference;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::const_pointer const_pointer;
		typedef typename base_type::iterator iterator;
		typedef typename base_type::const_iterator const_iterator;
		typedef typename base_type::reverse_iterator reverse_iterator;
		typedef typename base_type::const_reverse_iterator const_reverse_iterator;

	public:
		set() : set(key_compare()) {}
		explicit set(const key_compare& _compare, const allocator_type& _allocator = allocator_type());
	};

	template<typename Key, typename Compare = less<Key>, typename Allocator = allocator
		, typename DataStructure = avltree<Key, Key, Compare, Allocator, use_self<Key>, false, false> >
	class multiset : public DataStructure
	{
	private:
		typedef multiset<Key, Compare, Allocator, DataStructure> this_type;
		typedef DataStructure base_type;
	public:
		typedef typename base_type::key_type key_type;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::size_type size_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::key_compare key_compare;
		typedef typename base_type::key_compare value_compare;
		typedef typename base_type::allocator_type allocator_type;
		typedef typename base_type::reference reference;
		typedef typename base_type::const_reference const_reference;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::const_pointer const_pointer;
		typedef typename base_type::iterator iterator;
		typedef typename base_type::const_iterator const_iterator;
		typedef typename base_type::reverse_iterator reverse_iterator;
		typedef typename base_type::const_reverse_iterator const_reverse_iterator;

	public:
		multiset() : multiset(key_compare()) {}
		explicit multiset(const key_compare& _compare, const allocator_type& _allocator = allocator_type());
	};
}

namespace cckit
{
#pragma region set<Key, Compare, Allocator, DataStructure>
	template<typename Key, typename Compare, typename Allocator, typename DataStructure>
	set<Key, Compare, Allocator, DataStructure>::set(const key_compare& _compare, const allocator_type& _allocator)
		: base_type(_compare, _allocator)
	{}
#pragma endregion set<Key, Compare, Allocator, DataStructure>

#pragma region multiset<Key, Compare, Allocator, DataStructure>
	template<typename Key, typename Compare, typename Allocator, typename DataStructure>
	multiset<Key, Compare, Allocator, DataStructure>::multiset(const key_compare& _compare, const allocator_type& _allocator)
		: base_type(_compare, _allocator)
	{}
#pragma endregion multiset<Key, Compare, Allocator, DataStructure>
}

#endif // !CCKIT_SET_H