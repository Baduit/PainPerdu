#pragma once

#include <type_traits>
#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>

namespace Common
{

template<std::integral T>
class BasicCounter
{
	public:
		// Rule of 5 (+1)
		constexpr BasicCounter() = default;

		constexpr BasicCounter(const BasicCounter& other) { _value = other.get(); };
		constexpr BasicCounter& operator=(const BasicCounter& other) { _value = other.get(); return *this; };

		constexpr BasicCounter(BasicCounter&& other) { _value = std::move(other._value); };
		constexpr BasicCounter operator=(BasicCounter&& other) { _value = std::move(other._value); return *this; };

		virtual ~BasicCounter() = default;

		// std::integral values are normally about the same size as a pointer or shorter so no need to pass them as const&
		constexpr T       operator*() const { return _value; }
		constexpr T       get() const { return _value; }
		constexpr void    reset() { _value = 0; }

		constexpr BasicCounter&   operator++() { ++_value; return *this; }
		constexpr BasicCounter&   operator++(int) { _value++; return *this; }

		constexpr BasicCounter&   operator--() { --_value; return *this; }
		constexpr BasicCounter&   operator--(int) { _value--; return *this; }


	private:
		T   _value = 0;
};

template<std::integral T>
class BasicAtomicCounter
{
	public:
		// Rule of 5 (+1)
		constexpr BasicAtomicCounter() = default;

		BasicAtomicCounter(const BasicAtomicCounter& other) { _value = other.get(); }
		BasicAtomicCounter& operator=(const BasicAtomicCounter& other) { _value = other.get(); return *this; }

		BasicAtomicCounter(BasicAtomicCounter&& other) { _value = other.get(); }
		BasicAtomicCounter operator=(BasicAtomicCounter&& other) { _value = other.get(); return *this; }

		virtual ~BasicAtomicCounter() = default;

		// with an atomic value we can only access a copy of the value (and return the atomic directly does not seem like a good idea)
		T       operator*() const { return _value.load(); }
		T       get() const { return _value.load(); }
		void    reset() { _value.store(0); }

		BasicAtomicCounter&   operator++() { _value.fetch_add(1); return *this; }
		BasicAtomicCounter&   operator++(int) { _value.fetch_add(1); return *this; }

		BasicAtomicCounter&   operator--() { _value.fetch_sub(1); return *this; }
		BasicAtomicCounter&   operator--(int) { _value.fetch_sub(1); return *this; }


	private:
		std::atomic<T>   _value { 0 };
};

template<std::integral T>
class BasicMutexCounter
{
	public:
		// Rule of 5 (+1)
		constexpr BasicMutexCounter() = default;

		BasicMutexCounter(const BasicMutexCounter& other)
		{
			std::lock_guard	lock(_mutex);
			_value = other.get();
		}

		BasicMutexCounter& operator=(const BasicMutexCounter& other)
		{
			std::lock_guard	lock(_mutex);
			_value = other.get();
			return *this;
		}

		BasicMutexCounter(BasicMutexCounter&& other)
		{
			std::lock_guard	lock(_mutex);
			_value = other.get();
		}
		BasicMutexCounter operator=(BasicMutexCounter&& other)
		{
			std::lock_guard	lock(_mutex);
			_value = other.get();
			return *this;
		}

		virtual ~BasicMutexCounter() = default;

		// never return reference to something you want to protect with a mutex
		T		operator*() const { return get(); }

		T       get() const
		{
			std::lock_guard	lock(_mutex);
			return _value;
		}

		void    reset()
		{
			std::lock_guard	lock(_mutex);
			_value = 0;
		}

		BasicMutexCounter&   operator++()
		{
			std::lock_guard	lock(_mutex);
			++_value;
			return *this;
		}

		BasicMutexCounter&   operator++(int)
		{
			std::lock_guard	lock(_mutex);
			_value++;
			return *this;
		}

		BasicMutexCounter&   operator--()
		{
			std::lock_guard	lock(_mutex);
			--_value;
			return *this;
		}

		BasicMutexCounter&   operator--(int)
		{
			std::lock_guard	lock(_mutex);
			_value--;
			return *this;
		}


	private:
		T   				_value = 0;
		mutable std::mutex	_mutex;
};

template<std::integral T>
class BasicSharedMutexCounter
{
	public:
		// Rule of 5 (+1)
		constexpr BasicSharedMutexCounter() = default;

		BasicSharedMutexCounter(const BasicSharedMutexCounter& other)
		{
			std::lock_guard	lock(_mutex);
			_value = other.get();
		}

		BasicSharedMutexCounter& operator=(const BasicSharedMutexCounter& other)
		{
			std::lock_guard	lock(_mutex);
			_value = other.get();
			return *this;
		}

		BasicSharedMutexCounter(BasicSharedMutexCounter&& other)
		{
			std::lock_guard	lock(_mutex);
			_value = other.get();
		}
		BasicSharedMutexCounter operator=(BasicSharedMutexCounter&& other)
		{
			std::lock_guard	lock(_mutex);
			_value = other.get();
			return *this;
		}

		virtual ~BasicSharedMutexCounter() = default;

		// never return reference to something you want to protect with a mutex
		T		operator*() const { return get(); }
		
		T       get() const
		{
			std::shared_lock	lock(_mutex);
			return _value;
		}

		void    reset()
		{
			std::lock_guard	lock(_mutex);
			_value = 0;
		}

		BasicSharedMutexCounter&   operator++()
		{
			std::lock_guard	lock(_mutex);
			++_value;
			return *this;
		}

		BasicSharedMutexCounter&   operator++(int)
		{
			std::lock_guard	lock(_mutex);
			_value++;
			return *this;
		}

		BasicSharedMutexCounter&   operator--()
		{
			std::lock_guard	lock(_mutex);
			--_value;
			return *this;
		}

		BasicSharedMutexCounter&   operator--(int)
		{
			std::lock_guard	lock(_mutex);
			_value--;
			return *this;
		}


	private:
		T   						_value = 0;
		mutable std::shared_mutex	_mutex;
};

// aliases
using Counter = BasicCounter<int>;
using AtomicCounter = BasicAtomicCounter<int>;
using MutexCounter = BasicMutexCounter<int>;
using SharedMutexCounter = BasicSharedMutexCounter<int>;

}