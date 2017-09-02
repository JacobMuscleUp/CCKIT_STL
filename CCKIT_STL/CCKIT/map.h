#ifndef CCKIT_MAP_H
#define CCKIT_MAP_H

#include "internal/avltree.h"
#include "allocator.h"

namespace cckit
{
	template<typename Key, typename T, typename Compare = less<Key>, typename Allocator = allocator
		, typename DataStructure = avltree<Key, T, Compare, Allocator, use_first<pair<const Key, T> >, true, true> >
	class map : public DataStructure
	{
	private:
		typedef map<Key, T, Compare, Allocator, DataStructure> this_type;
		typedef DataStructure base_type;
	public:
		typedef typename base_type::key_type key_type;
		typedef T mapped_type;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::size_type size_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::key_compare key_compare;
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
		map() : map(key_compare()) {}
		explicit map(const key_compare& _compare, const allocator_type& _allocator = allocator_type());
	};

	template<typename Key, typename T, typename Compare = less<Key>, typename Allocator = allocator
		, typename DataStructure = avltree<Key, T, Compare, Allocator, use_first<pair<const Key, T> >, true, false> >
	class multimap : public DataStructure
	{
	private:
		typedef multimap<Key, T, Compare, Allocator, DataStructure> this_type;
		typedef DataStructure base_type;
	public:
		typedef typename base_type::key_type key_type;
		typedef T mapped_type;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::size_type size_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::key_compare key_compare;
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
		multimap() : multimap(key_compare()) {}
		explicit multimap(const key_compare& _compare, const allocator_type& _allocator = allocator_type());
	};
}

namespace cckit
{
#pragma region map<Key, T, Compare, Allocator, DataStructure>
	template<typename Key, typename T, typename Compare, typename Allocator, typename DataStructure>
	map<Key, T, Compare, Allocator, DataStructure>::map(const key_compare& _compare, const allocator_type& _allocator)
		: base_type(_compare, _allocator)
	{}
#pragma endregion map<Key, T, Compare, Allocator, DataStructure>

#pragma region multimap<Key, T, Compare, Allocator, DataStructure>
	template<typename Key, typename T, typename Compare, typename Allocator, typename DataStructure>
	multimap<Key, T, Compare, Allocator, DataStructure>::multimap(const key_compare& _compare, const allocator_type& _allocator)
		: base_type(_compare, _allocator)
	{}
#pragma endregion multimap<Key, T, Compare, Allocator, DataStructure>
}

#endif // !CCKIT_MAP_H