#pragma once

#include <mutex>
#include <string>

namespace bench
{
namespace tests
{
namespace std
{

class mutex : public test_template<mutex>
{
public:
    mutex(bench::test_config config) : test_template(config) {}

    void run_iteration(unsigned long iteration)
    {
        ::std::lock_guard<::std::mutex> lock(_mutex);
        (void)lock; // unused
        _value = !_value;
    }

    static ::std::string name()
    {
        return "std_mutex";
    }

    static ::std::string description()
    {
        return "Lock/unlock a shared std::mutex by each thread.";
    }

    static bool size_dependent()
    {
        return false;
    }

private:
    // A mutex shared by all threads.
    static ::std::mutex _mutex;
    bool _value;
};

::std::mutex mutex::_mutex{};

} // namespace std
} // namespace tests
} // namespace bench
