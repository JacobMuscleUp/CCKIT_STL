#ifndef CCKIT_ITERATOR_H
#define CCKIT_ITERATOR_H

#include <cstdlib>
#include "internal/config.h"
#include "type_traits.h"

namespace cckit
{
#pragma region iterator_tag
	struct input_iterator_tag { };
	struct output_iterator_tag { };
	struct forward_iterator_tag : public input_iterator_tag { };
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };
	struct contiguous_iterator_tag : public random_access_iterator_tag { }; 
#pragma endregion iterator_tag

#pragma region iterator
	template<typename Category, typename T, typename Distance = ptrdiff_t
		, typename Pointer = T*, typename Reference = T&>
		struct iterator
	{
		typedef Category iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef Pointer pointer;
		typedef Reference reference;
	};
#pragma endregion iterator

#pragma region IteratorTraitsBase
	template<typename, typename = void>
		struct IteratorTraitsBase
	{};
	template<typename Iterator>
	struct IteratorTraitsBase<Iterator, void_t<
		typename Iterator::iterator_category,
		typename Iterator::value_type,
		typename Iterator::difference_type,
		typename Iterator::pointer,
		typename Iterator::reference
	> >
	{	
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
	};
#pragma endregion IteratorTraitsBase

#pragma region iterator_traits
	template<typename Iterator>
	struct iterator_traits : IteratorTraitsBase<Iterator>
	{};
	template<typename T>
	struct iterator_traits<T*>
	{
		typedef random_access_iterator_tag iterator_category;    
		typedef T value_type;       
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
	};
	template<typename T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef const T* pointer;
		typedef const T& reference;
	};
#pragma endregion iterator_traits

#pragma region reverse_iterator
	template<typename Iterator>
	class reverse_iterator;

	template<typename Iterator0, typename Iterator1>
	CCKIT_CONSTEXPR bool operator==(const reverse_iterator<Iterator0> _lhs, const reverse_iterator<Iterator1> _rhs);
	template<typename Iterator0, typename Iterator1>
	CCKIT_CONSTEXPR bool operator!=(const reverse_iterator<Iterator0> _lhs, const reverse_iterator<Iterator1> _rhs);

	template<typename Iterator>
	class reverse_iterator : public iterator<
		typename iterator_traits<Iterator>::iterator_category
		, typename iterator_traits<Iterator>::value_type
		, typename iterator_traits<Iterator>::difference_type
		, typename iterator_traits<Iterator>::pointer
		, typename iterator_traits<Iterator>::reference>
	{
	private:
		typedef reverse_iterator<Iterator> this_type;
	public:
		typedef Iterator iterator_type;

	public:
		CCKIT_CONSTEXPR reverse_iterator();
		CCKIT_CONSTEXPR explicit reverse_iterator(iterator_type _base);
		template<typename Iterator0>
		CCKIT_CONSTEXPR reverse_iterator(const reverse_iterator<Iterator0>& _src);
		template<typename Iterator0>
		this_type& operator=(const reverse_iterator<Iterator0>& _rhs);

		CCKIT_CONSTEXPR iterator_type base() const;

		CCKIT_CONSTEXPR reference operator*() const;
		CCKIT_CONSTEXPR pointer operator->() const;
		CCKIT_CONSTEXPR reference operator[](difference_type _offset) const;

		this_type& operator++();
		this_type operator++(int);
		this_type& operator--();
		this_type operator--(int);

		template<typename Iterator0, typename Iterator1>
		friend CCKIT_CONSTEXPR bool operator== <>(const reverse_iterator<Iterator0> _lhs, const reverse_iterator<Iterator1> _rhs);
		template<typename Iterator0, typename Iterator1>
		friend CCKIT_CONSTEXPR bool operator!= <>(const reverse_iterator<Iterator0> _lhs, const reverse_iterator<Iterator1> _rhs);

	protected:
		iterator_type mBase;
	};

	template<typename Iterator>
	inline CCKIT_CONSTEXPR reverse_iterator<Iterator>::reverse_iterator()
		: mBase()
	{}
	template<typename Iterator>
	inline CCKIT_CONSTEXPR reverse_iterator<Iterator>::reverse_iterator(iterator_type _base)
		: mBase(_base)
	{}
	template<typename Iterator>
	template<typename Iterator0>
	inline CCKIT_CONSTEXPR reverse_iterator<Iterator>::reverse_iterator(const reverse_iterator<Iterator0>& _src)
		: mBase(_src.mBase)
	{}

	template<typename Iterator>
	template<typename Iterator0>
	inline typename reverse_iterator<Iterator>::this_type& 
		reverse_iterator<Iterator>::operator=(const reverse_iterator<Iterator0>& _rhs)
	{
		mBase = _rhs.mBase;
		return *this;
	}

	template<typename Iterator>
	inline CCKIT_CONSTEXPR typename reverse_iterator<Iterator>::iterator_type
		reverse_iterator<Iterator>::base() const
	{
		return mBase;
	}

	template<typename Iterator>
	inline CCKIT_CONSTEXPR typename reverse_iterator<Iterator>::reference
		reverse_iterator<Iterator>::operator*() const
	{
		--const_cast<this_type*>(this)->mBase;
		return *(const_cast<this_type*>(this)->mBase++);
		/*iterator_type temp = mBase;
		return *--temp;*/
	}
	template<typename Iterator>
	inline CCKIT_CONSTEXPR typename reverse_iterator<Iterator>::pointer
		reverse_iterator<Iterator>::operator->() const
	{
		return &(**this);
	}

	template<typename Iterator>
	inline CCKIT_CONSTEXPR typename reverse_iterator<Iterator>::reference 
		reverse_iterator<Iterator>::operator[](difference_type _offset) const
	{
		return *(mBase - (_offset + 1));
	}

	template<typename Iterator>
	inline typename reverse_iterator<Iterator>::this_type& 
		reverse_iterator<Iterator>::operator++()
	{
		--mBase;
		return *this;
	}
	template<typename Iterator>
	inline typename reverse_iterator<Iterator>::this_type
		reverse_iterator<Iterator>::operator++(int)
	{
		this_type temp = ++*this;
		return --temp;
	}
	template<typename Iterator>
	inline typename reverse_iterator<Iterator>::this_type&
		reverse_iterator<Iterator>::operator--()
	{
		++mBase;
		return *this;
	}
	template<typename Iterator>
	inline typename reverse_iterator<Iterator>::this_type
		reverse_iterator<Iterator>::operator--(int)
	{
		this_type temp = --*this;
		return ++temp;
	}

	template<typename Iterator0, typename Iterator1>
	CCKIT_CONSTEXPR bool operator==(const reverse_iterator<Iterator0> _lhs, const reverse_iterator<Iterator1> _rhs)
	{
		return _lhs.mBase == _rhs.mBase;
	}
	template<typename Iterator0, typename Iterator1>
	CCKIT_CONSTEXPR bool operator!=(const reverse_iterator<Iterator0> _lhs, const reverse_iterator<Iterator1> _rhs)
	{
		return _lhs.mBase != _rhs.mBase;
	}
#pragma endregion reverse_iterator
	
#pragma region is_iterator
	template<typename T, typename = void>
	struct is_iterator 
		: false_type {};
	template<typename T>
	struct is_iterator<T, void_t<typename iterator_traits<T>::iterator_category> >
		: true_type {};
#pragma endregion is_iterator

#pragma region distance
	template <typename InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator _first, InputIterator _last)
	{
		typedef typename iterator_traits<InputIterator>::iterator_category iterator_category;
		return Distance(_first, _last, iterator_category());
	}
	template <typename InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type 
		Distance(InputIterator _first, InputIterator _last, input_iterator_tag)
	{
		typename iterator_traits<InputIterator>::difference_type distance = 0;

		while (_first != _last) {
			++_first;
			++distance;
		}
		return distance;
	}
	template <typename RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
		Distance(RandomAccessIterator _first, RandomAccessIterator _last, random_access_iterator_tag)
	{
		return static_cast<typename iterator_traits<RandomAccessIterator>::difference_type>(_last - _first);
	}
#pragma endregion distance

#pragma region advance
	template <typename InputIterator, typename Distance>
	inline void advance(InputIterator& _iter, Distance _dist)
	{
		typedef typename iterator_traits<InputIterator>::iterator_category iterator_category;
		Advance(_iter, _dist, iterator_category());
	}
	template<typename InputIterator, typename Distance>
	inline void Advance(InputIterator& _iter, Distance _dist, input_iterator_tag)
	{
		while (n--)
			++_dist;
	}
	template<typename BidirectionalIterator, typename Distance>
	inline void Advance(BidirectionalIterator& _iter, Distance _dist, bidirectional_iterator_tag)
	{
		if (_dist > 0)
			while (_dist--)
				++_iter;
		else 
			while (_dist++)
				--_iter;
	}
	template<typename RandomAccessIterator, typename Distance>
	inline void Advance(RandomAccessIterator& _iter, Distance _dist, random_access_iterator_tag)
	{
		_iter += _dist;
	}
#pragma endregion advance

	template<typename InputIterator>
	inline InputIterator next(InputIterator _iter, typename iterator_traits<InputIterator>::difference_type _dist = 1)
	{
		advance(_iter, _dist);
		return _iter;
	}

	template<typename InputIterator>
	inline InputIterator prev(InputIterator _iter, typename iterator_traits<InputIterator>::difference_type _dist = 1)
	{
		advance(_iter, -_dist);
		return _iter;
	}
}

#endif // !CCKIT_ITERATOR_H