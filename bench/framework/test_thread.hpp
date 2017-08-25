#pragma once

#include <bench/core/clock.hpp>
#include <bench/core/test_instance.hpp>
#include <bench/framework/thread_synchronizer.hpp>
#include <cstdint>
#include <iostream>
#include <thread>

namespace bench
{
namespace framework
{

class test_thread
{
public:
    explicit test_thread(test_instance & test, thread_synchronizer<test_thread> & synchronizer)
        : _test_loop(create_test_loop(test)), _running(true), _synchronizer(synchronizer),
          _thread(std::thread(&test_thread::main, this))
    {
    }

    ~test_thread()
    {
        if (_thread.joinable()) _thread.join();
        assert(_running == false);
    }

    std::uint32_t iterations() const
    {
        return _test_loop->iterations();
    }

    std::uint32_t successes() const
    {
        return _test_loop->successes();
    }

    std::uint32_t failures() const
    {
        return _test_loop->failures();
    }
    double avg_duration()
    {
        return _test_loop->avg_duration();
    }

    void setup()
    {
        _test_loop->setup();
    }

    void test()
    {
        _test_loop->run();
    }

    void cleanup()
    {
        _test_loop->cleanup();
    }

    void terminate()
    {
        _running = false;
    }

private:
    void main()
    {
        while (_running)
        {
            try
            {
                auto action = _synchronizer.wait_order();
                (this->*action)();
            }
            catch (...)
            {
                _synchronizer.report_exception(std::current_exception());
            }
        }
    }

    std::unique_ptr<test_loop> _test_loop;
    bool _running;
    thread_synchronizer<test_thread> & _synchronizer;
    std::thread _thread;
};

using test_thread_collection = std::vector<std::unique_ptr<test_thread>>;

} // namespace framework
} // namespace bench
