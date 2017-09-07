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
		explicit set(const key_compare& _compare, const allocator_type& _allocator = allocator_type())
			: base_type(_compare, _allocator) {}
		explicit set(const allocator_type& _allocator)
			: base_type(_allocator) {}
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value>
			, typename = typename enable_if_t<is_same<typename iterator_traits<InputIterator>::value_type, value_type>::value> >
			set(InputIterator _first, InputIterator _last
				, const key_compare& _compare = key_compare(), const allocator_type& _allocator = allocator_type())
			: base_type(_first, _last, _compare, _allocator) {}
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value>
			, typename = typename enable_if_t<is_same<typename iterator_traits<InputIterator>::value_type, value_type>::value> >
			set(InputIterator _first, InputIterator _last, const allocator_type& _allocator)
			: set(_first, _last, key_compare(), _allocator) {}
		set(const this_type& _other, const allocator_type& _allocator = allocator_type())
			: base_type(_other, _allocator) {}
		set(this_type&& _other, const allocator_type& _allocator = allocator_type())
			: base_type(cckit::move(_other), _allocator) {}
		set(std::initializer_list<value_type> _ilist
			, const key_compare& _compare = key_compare(), const allocator_type& _allocator = allocator_type())
			: base_type(_ilist, _compare, _allocator) {}
		set(std::initializer_list<value_type> _ilist, const allocator_type& _allocator)
			: set(_ilist, key_compare(), _allocator) {}
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
		explicit multiset(const key_compare& _compare, const allocator_type& _allocator = allocator_type())
			: base_type(_compare, _allocator) {}
		explicit multiset(const allocator_type& _allocator)
			: base_type(_allocator) {}
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value>
			, typename = typename enable_if_t<is_same<typename iterator_traits<InputIterator>::value_type, value_type>::value> >
			multiset(InputIterator _first, InputIterator _last
				, const key_compare& _compare = key_compare(), const allocator_type& _allocator = allocator_type())
			: base_type(_first, _last, _compare, _allocator) {}
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value>
			, typename = typename enable_if_t<is_same<typename iterator_traits<InputIterator>::value_type, value_type>::value> >
			multiset(InputIterator _first, InputIterator _last, const allocator_type& _allocator)
			: multiset(_first, _last, key_compare(), _allocator) {}
		multiset(const this_type& _other, const allocator_type& _allocator = allocator_type())
			: base_type(_other, _allocator) {}
		multiset(this_type&& _other, const allocator_type& _allocator = allocator_type())
			: base_type(cckit::move(_other), _allocator) {}
		multiset(std::initializer_list<value_type> _ilist
			, const key_compare& _compare = key_compare(), const allocator_type& _allocator = allocator_type())
			: base_type(_ilist, _compare, _allocator) {}
		multiset(std::initializer_list<value_type> _ilist, const allocator_type& _allocator)
			: multiset(_ilist, key_compare(), _allocator) {}
	};
}

namespace cckit
{
#pragma region set<Key, Compare, Allocator, DataStructure>
	/*template<typename Key, typename Compare, typename Allocator, typename DataStructure>
	set<Key, Compare, Allocator, DataStructure>::set(const key_compare& _compare, const allocator_type& _allocator)
		: base_type(_compare, _allocator)
	{}*/
#pragma endregion set<Key, Compare, Allocator, DataStructure>

#pragma region multiset<Key, Compare, Allocator, DataStructure>
	/*template<typename Key, typename Compare, typename Allocator, typename DataStructure>
	multiset<Key, Compare, Allocator, DataStructure>::multiset(const key_compare& _compare, const allocator_type& _allocator)
		: base_type(_compare, _allocator)
	{}*/
#pragma endregion multiset<Key, Compare, Allocator, DataStructure>
}

#endif // !CCKIT_SET_H