#pragma once

#include <bench/core/clock.hpp>

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

    unsigned long iterations() const
    {
        return _iterations;
    }

protected:
    void add_iteration()
    {
        _iterations++;
    }

private:
    volatile unsigned long _iterations;
};

} // namespace bench
