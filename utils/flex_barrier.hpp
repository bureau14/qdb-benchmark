#pragma once

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <stdexcept>

namespace utils
{
class flex_barrier
{
public:
    template <typename F>
    flex_barrier(int num_threads, F completion) throw(std::invalid_argument)
        : _thread_count(num_threads), _new_thread_count(num_threads), _num_waiting(0), _completion_fn(completion)
    {
        if (num_threads == 0)
        {
            throw std::invalid_argument("num_threads is 0");
        }
        std::atomic_init(&_num_to_leave, 0);
    }
    bool arrive_and_wait()
    {
        std::unique_lock<std::mutex> local_lock{_mutex};
        _idle.wait(local_lock, [this] { return _num_to_leave == 0; });
        ++_num_waiting;
        if (_num_waiting == _thread_count)
        {
            _num_to_leave = _thread_count;
            _ready.notify_all();
        }
        else
        {
            _ready.wait(local_lock, [this] { return _num_waiting == _thread_count; });
        }

        if (_num_to_leave == 1)
        {
            _thread_count = _new_thread_count;
            _num_waiting = 0;
            _thread_count = static_cast<unsigned int>(_completion_fn());
            assert(_thread_count != 0);
            --_num_to_leave;
            _idle.notify_all();
            return true;
        }
        --_num_to_leave;
        return false;
    }

private:
    int _thread_count;
    int _new_thread_count;

    std::mutex _mutex;
    std::condition_variable _idle;
    std::condition_variable _ready;
    int _num_waiting;
    std::atomic_int _num_to_leave;

    std::function<int()> _completion_fn;
    void reset(int num_threads)
    {
        if (num_threads == 0)
        {
            throw std::invalid_argument("num_threads is 0");
        }
        _new_thread_count = num_threads;
    }
    void on_countdown()
    {
        reset(_completion_fn());
    }
};
} // namespace utils