#ifndef CCKIT_STACK_H
#define CCKIT_STACK_H

#include "internal/config.h"
#include "list.h"
#include "memory.h"
#include "static_assert.h"

namespace cckit
{
	// stack is an adapter class that provides a LIFO interface
	// via wrapping a sequence that provides at least the following operations:
	//		push_back
	//		pop_back
	//		emplace_back
	//		back
	//		empty
	//		size
	// in practice this means vector, deque, string, list, intrusive_list. 
	template<typename T, typename Container = list<T> >
	class stack
	{
		typedef stack<T, Container> this_type;
	public:
		typedef Container container_type;
		typedef typename container_type::value_type value_type;
		typedef typename container_type::size_type size_type;
		typedef typename container_type::reference reference;
		typedef typename container_type::const_reference const_reference;

	public:
		explicit stack(const container_type& _container);
		explicit stack(container_type&& _container = container_type());
		stack(const this_type& _src);
		stack(this_type&& _src);
		template<typename Allocator>
		explicit stack(const Allocator& _allocator, enable_if_t<uses_allocator<container_type, Allocator>::value>* = nullptr);
		template<typename Allocator>
		stack(const container_type& _container, const Allocator& _allocator
			, enable_if_t<uses_allocator<container_type, Allocator>::value>* = nullptr);
		template<typename Allocator>
		stack(container_type&& _container, const Allocator& _allocator
			, enable_if_t<uses_allocator<container_type, Allocator>::value>* = nullptr);
		template<typename Allocator>
		stack(const this_type& _src, const Allocator& _allocator
			, enable_if_t<uses_allocator<container_type, Allocator>::value>* = nullptr);
		template<typename Allocator>
		stack(this_type&& _src, const Allocator& _allocator
			, enable_if_t<uses_allocator<container_type, Allocator>::value>* = nullptr);

		this_type& operator=(const this_type& _rhs);
		this_type& operator=(this_type&& _rhs);

		reference top();
		const_reference top() const;
		bool empty() const;
		size_type size() const;

		void push(const value_type& value);
		void push(value_type&& value);
		template<typename... Args>
		reference emplace(Args&&... _args);
		void pop();
		void swap(this_type& _other) CCKIT_NOEXCEPT;

		// ADDITIONAL OPERATION
		reference operator[](size_type _index);
		const_reference operator[](size_type _index) const;
		//! ADDITIONAL OPERATION

	private:
		container_type mContainer;
	};
}

namespace cckit
{
#pragma region stack<T, Container>::stack
	template<typename T, typename Container>
	inline stack<T, Container>::stack(const container_type& _container)
		: mContainer(_container)
	{}
	template<typename T, typename Container>
	inline stack<T, Container>::stack(container_type&& _container = container_type())
		: mContainer(move(_container))
	{}
	template<typename T, typename Container>
	inline stack<T, Container>::stack(const this_type& _src)
		: mContainer(_src.mContainer)
	{}
	template<typename T, typename Container>
	inline stack<T, Container>::stack(this_type&& _src)
		: mContainer(move(_src.mContainer))
	{}
	template<typename T, typename Container>
	template<typename Allocator>
	inline stack<T, Container>::stack(const Allocator& _allocator, enable_if_t<uses_allocator<container_type, Allocator>::value>*)
		: mContainer(_allocator)
	{}
	template<typename T, typename Container>
	template<typename Allocator>
	inline stack<T, Container>::stack(const container_type& _container, const Allocator& _allocator
		, enable_if_t<uses_allocator<container_type, Allocator>::value>*)
		: mContainer(_container, _allocator)
	{}
	template<typename T, typename Container>
	template<typename Allocator>
	inline stack<T, Container>::stack(container_type&& _container, const Allocator& _allocator
		, enable_if_t<uses_allocator<container_type, Allocator>::value>*)
		: mContainer(move(_container), _allocator)
	{}
	template<typename T, typename Container>
	template<typename Allocator>
	inline stack<T, Container>::stack(const this_type& _src, const Allocator& _allocator
		, enable_if_t<uses_allocator<container_type, Allocator>::value>*)
		: mContainer(_src.mContainer, _allocator)
	{}
	template<typename T, typename Container>
	template<typename Allocator>
	inline stack<T, Container>::stack(this_type&& _src, const Allocator& _allocator
		, enable_if_t<uses_allocator<container_type, Allocator>::value>*)
		: mContainer(move(_src.mContainer), _allocator)
	{}
#pragma endregion stack<T, Container>::stack

#pragma region stack<T, Container>::operator=
	template<typename T, typename Container>
	inline typename stack<T, Container>::this_type& 
		stack<T, Container>::operator=(const this_type& _rhs)
	{
		mContainer = _rhs.mContainer;
		return *this;
	}
	template<typename T, typename Container>
	inline typename stack<T, Container>::this_type&
		stack<T, Container>::operator=(this_type&& _rhs)
	{
		mContainer = move(_rhs.mContainer);
		return *this;
	}
#pragma endregion stack<T, Container>::operator=

#pragma region stack<T, Container>::top
	template<typename T, typename Container>
	inline typename stack<T, Container>::reference 
		stack<T, Container>::top()
	{
		return mContainer.back();
	}
	template<typename T, typename Container>
	inline typename stack<T, Container>::const_reference 
		stack<T, Container>::top() const
	{
		return mContainer.back();
	}
#pragma endregion stack<T, Container>::top

#pragma region stack<T, Container>::empty
	template<typename T, typename Container>
	inline bool stack<T, Container>::empty() const
	{
		return mContainer.empty();
	}
#pragma endregion stack<T, Container>::empty
	
#pragma region stack<T, Container>::size
	template<typename T, typename Container>
	inline typename stack<T, Container>::size_type 
		stack<T, Container>::size() const
	{
		return mContainer.size();
	}
#pragma endregion stack<T, Container>::size

#pragma region stack<T, Container>::push
	template<typename T, typename Container>
	inline void stack<T, Container>::push(const value_type& _val)
	{
		mContainer.push_back(_val);
	}
	template<typename T, typename Container>
	inline void stack<T, Container>::push(value_type&& _val)
	{
		mContainer.push_back(move(_val));
	}
#pragma endregion stack<T, Container>::push
	
#pragma region stack<T, Container>::emplace
	template<typename T, typename Container>
	template<typename... Args>
	inline typename stack<T, Container>::reference 
		stack<T, Container>::emplace(Args&&... _args)
	{
		return mContainer.emplace_back(forward<Args>(_args)...);
	}
#pragma endregion stack<T, Container>::emplace
	
#pragma region stack<T, Container>::pop
	template<typename T, typename Container>
	inline void stack<T, Container>::pop()
	{
		mContainer.pop_back();
	}
#pragma endregion stack<T, Container>::pop

#pragma region stack<T, Container>::swap
	template<typename T, typename Container>
	inline void stack<T, Container>::swap(this_type& _other) CCKIT_NOEXCEPT
	{
		mContainer.swap(_other.mContainer);
	}
#pragma endregion stack<T, Container>::swap

#pragma region stack<T, Container>::operator[]
	template<typename T, typename Container>
	inline typename stack<T, Container>::reference 
		stack<T, Container>::operator[](size_type _index)
	{
		STATIC_ASSERT((is_same<container_type, list<T, container_type::allocator_type> >::value), INVALID_CONTAINER_TYPE);
		return mContainer[_index];
	}
	template<typename T, typename Container>
		inline typename stack<T, Container>::const_reference
		stack<T, Container>::operator[](size_type _index) const
	{
		STATIC_ASSERT((is_same<container_type, list<T, container_type::allocator_type> >::value), INVALID_CONTAINER_TYPE);
		return mContainer[_index];
	}
#pragma endregion stack<T, Container>::operator[]
}

#endif // !CCKIT_STACK_H