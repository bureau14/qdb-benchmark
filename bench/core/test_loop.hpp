#pragma once

#include <bench/core/clock.hpp>
#include <atomic>
#include <cstdint>

namespace bench
{

class test_loop
{
public:
    test_loop() : _iterations(0)
    {
    }

    virtual ~test_loop()
    {
    }

    virtual void setup() = 0;
    virtual void run() = 0;
    virtual void cleanup() = 0;

    std::uint32_t iterations() const noexcept
    {
        return _iterations;
    }

    std::uint32_t real_count() const noexcept
    {
        return _iterations - _error_count;
    }

protected:
    void add_iteration() noexcept
    {
        _iterations++;
    }

    void add_error() noexcept
    {
        _error_count++;
    }

    bool exceed_error_limit() const noexcept
    {
        return (_iterations / _max_error_percentage) < _error_count;
    }

private:
    std::atomic_uint32_t _iterations;
    std::atomic_uint32_t _error_count;
    std::uint32_t _max_error_percentage = 10;
};

} // namespace bench
