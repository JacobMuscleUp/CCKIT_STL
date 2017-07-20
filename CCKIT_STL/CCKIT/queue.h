#ifndef CCKIT_QUEUE_H
#define CCKIT_QUEUE_H

#include "internal/config.h"
#include "list.h"

namespace cckit
{
	// queue is an adapter class that provides a FIFO interface
	// via wrapping a sequence that provides at least the following operations:
	//		push_back
	//		pop_front
	//		emplace_back
	//		back
	//		front
	//		empty
	//		size
	// in practice this means vector, deque, string, list, intrusive_list. 
	template<typename T, typename Container = list<T> >
	class queue
	{
		typedef queue<T, Container> this_type;
	public:
		typedef Container container_type;
		typedef typename container_type::value_type value_type;
		typedef typename container_type::size_type size_type;
		typedef typename container_type::reference reference;
		typedef typename container_type::const_reference const_reference;

	public:
		explicit queue(const container_type& _container);
		explicit queue(container_type&& _container = container_type());
		queue(const this_type& _src);
		queue(this_type&& _src);
		template<typename Allocator>
		explicit queue(const Allocator& _allocator, enable_if_t<uses_allocator<container_type, Allocator>::value>* = nullptr);
		template<typename Allocator>
		queue(const container_type& _container, const Allocator& _allocator
			, enable_if_t<uses_allocator<container_type, Allocator>::value>* = nullptr);
		template<typename Allocator>
		queue(container_type&& _container, const Allocator& _allocator
			, enable_if_t<uses_allocator<container_type, Allocator>::value>* = nullptr);
		template<typename Allocator>
		queue(const this_type& _src, const Allocator& _allocator
			, enable_if_t<uses_allocator<container_type, Allocator>::value>* = nullptr);
		template<typename Allocator>
		queue(this_type&& _src, const Allocator& _allocator
			, enable_if_t<uses_allocator<container_type, Allocator>::value>* = nullptr);

		this_type& operator=(const this_type& _rhs);
		this_type& operator=(this_type&& _rhs);

		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
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
#pragma region queue<T, Container>::stack
	template<typename T, typename Container>
	inline queue<T, Container>::queue(const container_type& _container)
		: mContainer(_container)
	{}
	template<typename T, typename Container>
	inline queue<T, Container>::queue(container_type&& _container = container_type())
		: mContainer(cckit::move(_container))
	{}
	template<typename T, typename Container>
	inline queue<T, Container>::queue(const this_type& _src)
		: mContainer(_src.mContainer)
	{}
	template<typename T, typename Container>
	inline queue<T, Container>::queue(this_type&& _src)
		: mContainer(cckit::move(_src.mContainer))
	{}
	template<typename T, typename Container>
	template<typename Allocator>
	inline queue<T, Container>::queue(const Allocator& _allocator, enable_if_t<uses_allocator<container_type, Allocator>::value>*)
		: mContainer(_allocator)
	{}
	template<typename T, typename Container>
	template<typename Allocator>
	inline queue<T, Container>::queue(const container_type& _container, const Allocator& _allocator
		, enable_if_t<uses_allocator<container_type, Allocator>::value>*)
		: mContainer(_container, _allocator)
	{}
	template<typename T, typename Container>
	template<typename Allocator>
	inline queue<T, Container>::queue(container_type&& _container, const Allocator& _allocator
		, enable_if_t<uses_allocator<container_type, Allocator>::value>*)
		: mContainer(move(_container), _allocator)
	{}
	template<typename T, typename Container>
	template<typename Allocator>
	inline queue<T, Container>::queue(const this_type& _src, const Allocator& _allocator
		, enable_if_t<uses_allocator<container_type, Allocator>::value>*)
		: mContainer(_src.mContainer, _allocator)
	{}
	template<typename T, typename Container>
	template<typename Allocator>
	inline queue<T, Container>::queue(this_type&& _src, const Allocator& _allocator
		, enable_if_t<uses_allocator<container_type, Allocator>::value>*)
		: mContainer(move(_src.mContainer), _allocator)
	{}
#pragma endregion queue<T, Container>::queue

#pragma region queue<T, Container>::operator=
	template<typename T, typename Container>
	inline typename queue<T, Container>::this_type&
		queue<T, Container>::operator=(const this_type& _rhs)
	{
		mContainer = _rhs.mContainer;
		return *this;
	}
	template<typename T, typename Container>
	inline typename queue<T, Container>::this_type&
		queue<T, Container>::operator=(this_type&& _rhs)
	{
		mContainer = move(_rhs.mContainer);
		return *this;
	}
#pragma endregion queue<T, Container>::operator=

#pragma region queue<T, Container>::front
	template<typename T, typename Container>
	inline typename queue<T, Container>::reference
		queue<T, Container>::front()
	{
		return mContainer.front();
	}
	template<typename T, typename Container>
	inline typename queue<T, Container>::const_reference
		queue<T, Container>::front() const
	{
		return mContainer.front();
	}
#pragma endregion queue<T, Container>::front

#pragma region queue<T, Container>::back
	template<typename T, typename Container>
	inline typename queue<T, Container>::reference
		queue<T, Container>::back()
	{
		return mContainer.back();
	}
	template<typename T, typename Container>
	inline typename queue<T, Container>::const_reference
		queue<T, Container>::back() const
	{
		return mContainer.back();
	}
#pragma endregion queue<T, Container>::back

#pragma region queue<T, Container>::empty
	template<typename T, typename Container>
	inline bool queue<T, Container>::empty() const
	{
		return mContainer.empty();
	}
#pragma endregion queue<T, Container>::empty

#pragma region queue<T, Container>::size
	template<typename T, typename Container>
	inline typename queue<T, Container>::size_type
		queue<T, Container>::size() const
	{
		return mContainer.size();
	}
#pragma endregion queue<T, Container>::size

#pragma region queue<T, Container>::push
	template<typename T, typename Container>
	inline void queue<T, Container>::push(const value_type& _val)
	{
		mContainer.push_back(_val);
	}
	template<typename T, typename Container>
	inline void queue<T, Container>::push(value_type&& _val)
	{
		mContainer.push_back(move(_val));
	}
#pragma endregion queue<T, Container>::push

#pragma region queue<T, Container>::emplace
	template<typename T, typename Container>
	template<typename... Args>
	inline typename queue<T, Container>::reference
		queue<T, Container>::emplace(Args&&... _args)
	{
		return mContainer.emplace_back(forward<Args>(_args)...);
	}
#pragma endregion queue<T, Container>::emplace

#pragma region queue<T, Container>::pop
	template<typename T, typename Container>
	inline void queue<T, Container>::pop()
	{
		mContainer.pop_front();
	}
#pragma endregion queue<T, Container>::pop

#pragma region queue<T, Container>::swap
	template<typename T, typename Container>
	inline void queue<T, Container>::swap(this_type& _other) CCKIT_NOEXCEPT
	{
		mContainer.swap(_other.mContainer);
	}
#pragma endregion queue<T, Container>::swap

#pragma region queue<T, Container>::operator[]
	template<typename T, typename Container>
	inline typename queue<T, Container>::reference
		queue<T, Container>::operator[](size_type _index)
	{
		STATIC_ASSERT((is_same<container_type, list<T, container_type::allocator_type> >::value), INVALID_CONTAINER_TYPE);
		return mContainer[_index];
	}
	template<typename T, typename Container>
	inline typename queue<T, Container>::const_reference
		queue<T, Container>::operator[](size_type _index) const
	{
		STATIC_ASSERT((is_same<container_type, list<T, container_type::allocator_type> >::value), INVALID_CONTAINER_TYPE);
		return mContainer[_index];
	}
#pragma endregion queue<T, Container>::operator[]
}

#endif // !CCKIT_QUEUE_H