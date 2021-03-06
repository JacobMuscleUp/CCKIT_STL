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
		explicit map(const key_compare& _compare, const allocator_type& _allocator = allocator_type())
			: base_type(_compare, _allocator) {}
		explicit map(const allocator_type& _allocator)
			: base_type(_allocator) {}
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value>
			, typename = typename enable_if_t<is_same<typename iterator_traits<InputIterator>::value_type, value_type>::value> >
			map(InputIterator _first, InputIterator _last
				, const key_compare& _compare = key_compare(), const allocator_type& _allocator = allocator_type())
			: base_type(_first, _last, _compare, _allocator) {}
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value>
			, typename = typename enable_if_t<is_same<typename iterator_traits<InputIterator>::value_type, value_type>::value> >
			map(InputIterator _first, InputIterator _last, const allocator_type& _allocator)
			: map(_first, _last, key_compare(), _allocator) {}
		map(const this_type& _other, const allocator_type& _allocator = allocator_type())
			: base_type(_other, _allocator) {}
		map(this_type&& _other, const allocator_type& _allocator = allocator_type())
			: base_type(cckit::move(_other), _allocator) {}
		map(std::initializer_list<value_type> _ilist
			, const key_compare& _compare = key_compare(), const allocator_type& _allocator = allocator_type())
			: base_type(_ilist, _compare, _allocator) {}
		map(std::initializer_list<value_type> _ilist, const allocator_type& _allocator)
			: map(_ilist, key_compare(), _allocator) {}

		this_type& operator=(const this_type& _rhs) { static_cast<base_type&>(*this) = _rhs; return *this; }
		this_type& operator=(this_type&& _rhs) { static_cast<base_type&>(*this) = cckit::forward<this_type>(_rhs); return *this; }
		this_type& operator=(std::initializer_list<value_type> _ilist) { static_cast<base_type&>(*this) = _ilist; return *this; }

		template<typename... Args>
		pair<iterator, bool> try_emplace(Args&&... _args)
		{
			key_type key = cckit::arg_at<0>(cckit::forward<Args>(_args)...);
			iterator lowerBound = lower_bound(key);
			if (lowerBound == end() || key_comp()(key, lowerBound->first))
				return pair<iterator, bool>(emplace_hint(lowerBound, cckit::forward<Args>(_args)...), true);
			return pair<iterator, bool>(lowerBound, false);
		}

		template<typename U>
		pair<iterator, bool> insert_or_assign(const key_type& _key, U&& _obj)
		{
			iterator lowerBound = lower_bound(_key);
			if (lowerBound == end() || key_comp()(_key, lowerBound->first))
				return pair<iterator, bool>(emplace_hint(lowerBound, _key, cckit::forward<U>(_obj)), true);
			lowerBound->second = cckit::forward<U>(_obj);
			return pair<iterator, bool>(lowerBound, false);
		}

		mapped_type& at(const key_type& _key)
		{
			iterator lowerBound = lower_bound(_key);
			if (lowerBound == end() || key_comp()(_key, lowerBound->first))
				throw std::out_of_range("invalid key for map::at");
			return lowerBound->second;
		}
		const mapped_type& at(const key_type& _key) const { return (const_cast<this_type*>(this))->at(_key); }

		mapped_type& operator[](const key_type& _key)
		{
			return try_emplace(_key, mapped_type()).first->second;
		}
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
		explicit multimap(const key_compare& _compare, const allocator_type& _allocator = allocator_type())
			: base_type(_compare, _allocator) {}
		explicit multimap(const allocator_type& _allocator)
			: base_type(_allocator) {}
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value>
			, typename = typename enable_if_t<is_same<typename iterator_traits<InputIterator>::value_type, value_type>::value> >
			multimap(InputIterator _first, InputIterator _last
				, const key_compare& _compare = key_compare(), const allocator_type& _allocator = allocator_type())
			: base_type(_first, _last, _compare, _allocator) {}
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value>
			, typename = typename enable_if_t<is_same<typename iterator_traits<InputIterator>::value_type, value_type>::value> >
			multimap(InputIterator _first, InputIterator _last, const allocator_type& _allocator)
			: multimap(_first, _last, key_compare(), _allocator) {}
		multimap(const this_type& _other, const allocator_type& _allocator = allocator_type())
			: base_type(_other, _allocator) {}
		multimap(this_type&& _other, const allocator_type& _allocator = allocator_type())
			: base_type(cckit::move(_other), _allocator) {}
		multimap(std::initializer_list<value_type> _ilist
			, const key_compare& _compare = key_compare(), const allocator_type& _allocator = allocator_type())
			: base_type(_ilist, _compare, _allocator) {}
		multimap(std::initializer_list<value_type> _ilist, const allocator_type& _allocator)
			: multimap(_ilist, key_compare(), _allocator) {}

		this_type& operator=(const this_type& _rhs) { static_cast<base_type&>(*this) = _rhs; return *this; }
		this_type& operator=(this_type&& _rhs) { static_cast<base_type&>(*this) = cckit::forward<this_type>(_rhs); return *this; }
		this_type& operator=(std::initializer_list<value_type> _ilist) { static_cast<base_type&>(*this) = _ilist; return *this; }
	};
}

namespace cckit
{
#pragma region map<Key, T, Compare, Allocator, DataStructure>
	/*template<typename Key, typename T, typename Compare, typename Allocator, typename DataStructure>
	map<Key, T, Compare, Allocator, DataStructure>::map(const key_compare& _compare, const allocator_type& _allocator)
	: base_type(_compare, _allocator)
	{}*/
#pragma endregion map<Key, T, Compare, Allocator, DataStructure>

#pragma region multimap<Key, T, Compare, Allocator, DataStructure>
	/*template<typename Key, typename T, typename Compare, typename Allocator, typename DataStructure>
	multimap<Key, T, Compare, Allocator, DataStructure>::multimap(const key_compare& _compare, const allocator_type& _allocator)
	: base_type(_compare, _allocator)
	{}*/
#pragma endregion multimap<Key, T, Compare, Allocator, DataStructure>
}

#endif // !CCKIT_MAP_H