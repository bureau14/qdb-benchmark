#pragma once

#include <atomic>
#include <string>

namespace bench
{
namespace tests
{
namespace std
{

class atomic : public test_template<atomic>
{
public:
    atomic(bench::test_config config) : test_template(config) {}

    void run_iteration(unsigned long iteration)
    {
        ++_value;
    }

    static ::std::string name()
    {
        return "std_atomic";
    }

    static ::std::string description()
    {
        return "Increment a shared std::atomic<int> by each thread.";
    }

    static bool size_dependent()
    {
        return false;
    }

private:
    // An atomic shared by all threads.
    static ::std::atomic<int> _value;
};

::std::atomic<int> atomic::_value{0};

} // namespace std
} // namespace tests
} // namespace bench
