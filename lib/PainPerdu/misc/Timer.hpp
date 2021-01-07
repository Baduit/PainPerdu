#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <vector>
#include <thread>
#include <future>
#include <atomic>
#include <condition_variable>
#include <type_traits>

using Duration = decltype(std::chrono::system_clock::now() - std::chrono::system_clock::now());

/*
** You can have the duration between now and the last reset of the clock. 
** It can't be paused.
*/
class SimpleClock
{
	public:
		SimpleClock() { reset(); };

		SimpleClock(const SimpleClock&) = default;
		SimpleClock& operator=(const SimpleClock&) = default;

		SimpleClock(SimpleClock&&) = default;
		SimpleClock& operator=(SimpleClock&&) = default;

		virtual ~SimpleClock() = default;

		void		reset() { _first = std::chrono::system_clock::now(); }
		
		Duration	get_time_duration() { return std::chrono::system_clock::now() - _first; }

		template<typename T>
		auto		get_duration_as() { return std::chrono::duration_cast<T>(get_time_duration()); }

		int64_t		get_time_nano_count() { return get_duration_as<std::chrono::nanoseconds>().count(); }

	private:
		std::chrono::system_clock::time_point	_first;
};

/*
** You can have the duration between now and the last reset of the clock. 
** It can be paused.
** Default status at construction: started.
*/
class AdvancedClock
{
	public:
		AdvancedClock() { reset(); }

		AdvancedClock(const AdvancedClock&) = default;
		AdvancedClock& operator=(const AdvancedClock&) = default;

		AdvancedClock(AdvancedClock&&) = default;
		AdvancedClock& operator=(AdvancedClock&&) = default;

		virtual ~AdvancedClock() = default;

		void		reset()
		{
			_run_timer.reset();
			_pause_timer.reset();
			_paused_time = {};
		}
		
		auto		get_time_duration()
		{
			return _run_timer.get_time_duration() - get_total_pause_time();
		}

		template<typename T>
		auto		get_duration_as()
		{
			return _run_timer.get_duration_as<T>() - get_total_pause_time_as<T>();
		}

		int64_t		get_time_nano_count()
		{
			return _run_timer.get_time_nano_count() - get_total_pause_time_nano_count();
		}

		void		pause()
		{
			if (!_pause_timer.has_value())
				_pause_timer.emplace();
		}

		void		start()
		{
			if (_pause_timer.has_value())
			{
				_paused_time += _pause_timer->get_time_duration();
				_pause_timer.reset();
			}
		}

		Duration	get_total_pause_time()
		{
			return (_pause_timer.has_value()) ? _paused_time + _pause_timer->get_time_duration() : _paused_time;
		}

		template<typename T>
		auto 	get_total_pause_time_as() { return std::chrono::duration_cast<T>(get_total_pause_time()); }


		int64_t	get_total_pause_time_nano_count() { return get_total_pause_time_as<std::chrono::nanoseconds>().count(); }

	private:
		SimpleClock						_run_timer;
		std::optional<SimpleClock>		_pause_timer;
		Duration						_paused_time;
};

/*
** Make a thread sleep, same as std::this_thread::sleep_for but cancelable.
*/
class Sleeper
{
	public:
		Sleeper() = default;

		Sleeper(const Sleeper&) = delete;
		Sleeper& operator=(const Sleeper&) = delete;

		Sleeper(Sleeper&&) = delete;
		Sleeper& operator=(Sleeper&&) = delete;

		virtual ~Sleeper()
		{
			cancel_all();
		}

		template<typename D>
		Sleeper(const D& duration)
		{
			sleep(duration);
		}

		template<typename D>
		void	operator()(const D& duration)
		{
			sleep(duration);
		}

		template<typename D>
		void	sleep(const D& duration)
		{
			std::unique_lock lock(_mutex);
			_cv.wait_for(lock, duration);
		}

		// Use it if you want to awake only one thread sleeping
		void	cancel_one()
		{
			_cv.notify_one();
		}

		// Use it if you cant to awake all the sleeping threads (one or more)
		void	cancel_all()
		{
			_cv.notify_all();
		}

	private:
		std::mutex				_mutex;
		std::condition_variable	_cv;
};

/*
** Execute an action at the end of the choosed duration in a loop in the current thread.
** For the moment it can't be paused.
*/
class Poller
{
	public:
		enum class LoopStatus
		{
			CONTINUE,
			STOP
		};

	public:
		Poller() = default;

		Poller(const Poller&) = delete;
		Poller& operator=(const Poller&) = delete;

		Poller(Poller&&) = delete;
		Poller& operator=(Poller&&) = delete;

		template<typename D, typename Cb>
		Poller(const D& duration, Cb&& cb)
		{
			start(duration, std::forward<Cb>(cb));
		}

		virtual ~Poller()
		{
			stop();
		}

		template<typename D, typename Cb>
		void	operator()(const D& duration, Cb&& cb)
		{
			start(duration, std::forward<Cb>(cb));
		}

		template<typename D, typename Cb>
		void	start(const D& duration, Cb&& cb)
		{
			while (!_stop)
			{
				_sleeper(duration);
				if (!_stop)
				{
					using CbReturnType = std::invoke_result_t<Cb>;
					if constexpr (std::is_same_v<CbReturnType, LoopStatus>)
					{
						if (cb() == LoopStatus::STOP)
							stop();
					}
					else
					{
						cb();
					}
				}
			}
		}

		void	stop()
		{
			_stop = true;
			_sleeper.cancel_all();
		}

	private:
		std::atomic<bool>	_stop { false };
		Sleeper				_sleeper;
};


enum class ActionOnDtor
{
	JOIN,
	STOP
};

/*
** Execute an action at the end of the choosed duration in an other thread.
** For the moment it can't be paused.
*/
class ThreadTimer
{
	public:
		ThreadTimer() = default;

		ThreadTimer(const ThreadTimer&) = delete;
		ThreadTimer& operator=(const ThreadTimer&) = delete;

		ThreadTimer(ThreadTimer&&) = delete;
		ThreadTimer& operator=(ThreadTimer&&) = delete;

		template<typename D, typename Cb>
		ThreadTimer(const D& duration, Cb&& cb)
		{
			start(duration, std::forward<Cb>(cb));
		}

		virtual ~ThreadTimer()
		{
			stop();
		}

		template<typename D, typename Cb>
		auto	start(const D& duration, Cb&& cb)
		{
			auto thread_cb = 
				[this, duration, moved_cb = std::forward<Cb>(cb)]()
				{
					_sleeper(duration);
					if (!_stop)
						return moved_cb();
					else
						return decltype(moved_cb())();
				};
			auto task = std::packaged_task<decltype(thread_cb())()>(std::forward<decltype(thread_cb)>(thread_cb));
			auto future = task.get_future();
		
			_thread = std::thread(std::move(task));
			return future;
		}

		void	join()
		{
			if (_thread.joinable())
			_thread.join();
		}

		// by calling this function while the timer is started, future used as result for the callback is unitialized
		void	stop()
		{
			_stop = true;
			_sleeper.cancel_all();
			join();
		}

	private:
		std::thread			_thread;
		std::atomic<bool>	_stop = false;
		Sleeper				_sleeper;
};

/*
** Execute an action at the end of the choosed duration in a loop in an other thread.
** For the moment it can't be paused.
*/
class LoopThreadTimer
{
	public:
		enum class LoopStatus
		{
			CONTINUE,
			STOP
		};

	public:
		LoopThreadTimer() = default;

		LoopThreadTimer(const LoopThreadTimer&) = delete;
		LoopThreadTimer& operator=(const LoopThreadTimer&) = delete;

		LoopThreadTimer(LoopThreadTimer&&) = delete;
		LoopThreadTimer& operator=(LoopThreadTimer&&) = delete;

		template<typename D, typename Cb>
		LoopThreadTimer(const D& duration, Cb&& cb)
		{
			start(duration, std::forward<Cb>(cb));
		}

		virtual ~LoopThreadTimer()
		{
			stop();
		}

		template<typename D, typename Cb>
		void	start(const D& duration, Cb&& cb)
		{
			auto thread_cb = 
				[this, duration, moved_cb = std::forward<Cb>(cb)]()
				{
					while (!_stop)
					{
						_sleeper(duration);
						if (!_stop)
						{
							using CbReturnType = std::invoke_result_t<Cb>;
							if constexpr (std::is_same_v<CbReturnType, LoopStatus>)
							{
								if (moved_cb() == LoopStatus::STOP)
									stop();
							}
							else
							{
								moved_cb();
							}
						}
					}
				};
			auto task = std::packaged_task<decltype(thread_cb())()>(std::forward<decltype(thread_cb)>(thread_cb));
		
			_thread = std::thread(std::move(task));
		}

		void	join()
		{
			if (_thread.joinable())
			_thread.join();
		}

		void	stop()
		{
			_stop = true;
			_sleeper.cancel_all();
			join();
		}

	private:
		std::thread			_thread;
		std::atomic<bool>	_stop { false };
		Sleeper				_sleeper;
};
 