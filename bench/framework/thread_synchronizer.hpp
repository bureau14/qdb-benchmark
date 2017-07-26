#pragma once

#include <cassert>
#include <condition_variable>
#include <exception>
#include <mutex>

namespace bench
{
namespace framework
{

template <typename Thread>
class thread_synchronizer
{
public:
    typedef void (Thread::*action_type)(void);

    thread_synchronizer() : _cycle{0}, _workers{0}, _ready_workers{0}
    {
    }

    void reset(std::size_t worker_count)
    {
        _cycle = 0;
        _workers = worker_count;
        _ready_workers = 0;
    }

    void send_order(action_type order)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        assert(_ready_workers >= _workers);
        assert(_ready_workers <= _workers);
        _order = order;
        _cycle++;
        _ready_workers = 0;
        _exception = nullptr;
        lock.unlock();
        _cv.notify_all();
    }

    void report_exception(std::exception_ptr eptr)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if (!_exception) _exception = eptr;
    }

    void rethrow()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if (_exception)
        {
            std::rethrow_exception(_exception);
        }
    }

    action_type wait_order()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _ready_workers++;
        std::size_t next_cycle = _cycle + 1;
        lock.unlock();
        _cv.notify_all();
        lock.lock();
        _cv.wait(lock, [=] { return _cycle >= next_cycle; });
        assert(_cycle == next_cycle);
        assert(_ready_workers <= _workers);
        return _order;
    }

    void wait_workers()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [=] { return _ready_workers >= _workers; });
        assert(_ready_workers == _workers);
    }

    template <class Rep, class Period>
    bool wait_workers_for(const std::chrono::duration<Rep, Period> & rel_time)
    {
        std::unique_lock<std::mutex> lock(_mutex);

        bool ok = _cv.wait_for(lock, rel_time, [=] { return _ready_workers >= _workers; });
        if (!ok)
        {
            assert(_ready_workers < _workers);
            return false;
        }
        else
        {
            assert(_ready_workers == _workers);
            return true;
        }
    }

private:
    std::size_t _cycle;
    std::size_t _workers, _ready_workers;
    action_type _order;
    std::mutex _mutex;
    std::condition_variable _cv;
    std::exception_ptr _exception;
};

} // namespace framework
} // namespace bench
