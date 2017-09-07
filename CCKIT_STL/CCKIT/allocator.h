#ifndef CCKIT_ALLOCATOR_H
#define CCKIT_ALLOCATOR_H

#include <new>
#include <cstdlib>
#include "internal/config.h"
#include "internal/afx_config.h"

namespace cckit
{
#pragma region allocator
	class allocator
	{
	public:
		allocator(const char* _pName = nullptr);
		allocator(const allocator& _src, const char* _pName = nullptr);
		allocator& operator=(const allocator& _rhs);

		void* allocate(size_t _sz, int _flags = 0) const;
		void* allocate(size_t _sz, size_t _alignment, size_t _offset, int _flags = 0) const;
		void deallocate(void* _pTarget) const;

		const char* get_name() const;
		void set_name(const char* _pName);

	protected:
	#if CCKIT_DEBUG
		const char* mpName;
	#endif
	};
	bool operator==(const allocator& a, const allocator& b);
	bool operator!=(const allocator& a, const allocator& b);
#pragma endregion allocator

#pragma region allocator_malloc
	class allocator_malloc : public allocator
	{
	public:
		void* allocate(size_t _sz, int _flags = 0) const;
		void* allocate(size_t _sz, size_t _alignment, size_t _offset, int _flags = 0) const;
		void deallocate(void* _pTarget) const;
	};
	bool operator==(const allocator_malloc& a, const allocator_malloc& b);
	bool operator!=(const allocator_malloc& a, const allocator_malloc& b);
#pragma endregion allocator_malloc

#pragma region allocator_dummy
	class allocator_dummy : public allocator {};
#pragma endregion allocator_dummy
}

namespace cckit
{
#pragma region allocator::allocator
	inline allocator::allocator(const char* _pName)
	{
	#if CCKIT_DEBUG
		mpName = (_pName) ? _pName : CCKIT_ALLOCATOR_DEFAULT_NAME;
	#endif
	}
	inline allocator::allocator(const allocator& _src, const char* _pName)
	{
	#if CCKIT_DEBUG
		if (_pName)
			mpName = (_pName) ? _pName : CCKIT_ALLOCATOR_DEFAULT_NAME;
		else
			mpName = _src.mpName;
	#endif
	}
#pragma endregion allocator::allocator

#pragma region allocator::operator=
	inline allocator& allocator::operator=(const allocator& _rhs)
	{
	#if CCKIT_DEBUG
		mpName = _rhs.mpName;
	#endif
		return *this;
	}
#pragma endregion allocator::operator=

#pragma region allocator::allocate
	inline void* allocator::allocate(size_t _sz, int _flags) const
	{
	#if CCKIT_DEBUG
		return ::operator new(_sz, __FILE__, __LINE__);
	#else
		return ::operator new(_sz);
	#endif
	}
	inline void* allocator::allocate(size_t _sz, size_t _alignment, size_t _offset, int _flags) const
	{
	#if CCKIT_DEBUG
		return ::operator new(_sz, __FILE__, __LINE__);
	#else
		return ::operator new(_sz);
	#endif
	}
#pragma endregion allocator::allocate

#pragma region allocator::deallocate
	void allocator::deallocate(void* _pTarget) const
	{
	#if CCKIT_DEBUG
		return ::operator delete(_pTarget, __FILE__, __LINE__);
	#else
		return ::operator delete(_pTarget);
	#endif
	}
#pragma endregion allocator::deallocate

#pragma region allocator::get_name
	const char* allocator::get_name() const
	{
	#if CCKIT_DEBUG
		return mpName;
	#else
		return CCKIT_ALLOCATOR_DEFAULT_NAME;
	#endif
	}
#pragma endregion allocator::get_name

#pragma region allocator::set_name
	void allocator::set_name(const char* _pName)
	{
	#if CCKIT_DEBUG
		mpName = _pName;
	#endif
	}
#pragma endregion allocator::set_name

	bool operator==(const allocator& a, const allocator& b) { return true; }
	bool operator!=(const allocator& a, const allocator& b) { return false; }

#pragma region allocator_malloc::allocate
	inline void* allocator_malloc::allocate(size_t _sz, int _flags) const
	{
		return std::malloc(_sz);
	}
	inline void* allocator_malloc::allocate(size_t _sz, size_t _alignment, size_t _offset, int _flags) const
	{
		return std::malloc(_sz);
	}
#pragma endregion allocator_malloc::allocate

#pragma region allocator_malloc::deallocate
	void allocator_malloc::deallocate(void* _pTarget) const
	{
		std::free(_pTarget);
	}
#pragma endregion allocator_malloc::deallocate

	bool operator==(const allocator_malloc& a, const allocator_malloc& b) { return true; }
	bool operator!=(const allocator_malloc& a, const allocator_malloc& b) { return false; }
}

#endif // !CCKIT_ALLOCATOR_H