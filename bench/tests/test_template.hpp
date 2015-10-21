#pragma once

#include <bench/core/test_class.hpp>
#include <bench/core/test_loop.hpp>
#include <utils/memory.hpp>

namespace bench
{
namespace tests
{

template <typename Derived> // CRTP
class test_template : public bench::test_loop
{
public:
    class test_class : public bench::test_class
    {
    public:
        test_class()
        {
            name = Derived::name();
            description = Derived::description();
            size_dependent = Derived::size_dependent();
        }

        ::std::unique_ptr<bench::test_loop> create_loop(bench::test_config config) const override
        {
            return utils::make_unique<Derived>(config);
        }

        probe_collection create_probes(bench::test_config config) const override
        {
            return Derived::create_probes(config);
        }
    };

    static probe_collection create_probes(bench::test_config config)
    {
        return {};
    }

    test_template(test_config config) : _config(config), _prepared_iterations(0)
    {
    }

    void setup() override
    {
    }

    void run() override
    {
        if (_prepared_iterations > 0)
            run_n_iterations(_prepared_iterations);
        else
            run_iteration_until(clock::now() + _config.duration);
    }

    void cleanup() override
    {
    }

protected:
    template <typename Function>
    void cleanup_each(Function function)
    {
        for (unsigned long i = 0; i < test_loop::iterations(); i++)
        {
            function(i);
        }
    }

    template <typename Function>
    void setup_each(Function function)
    {
        clock::time_point timeout = clock::now() + _config.duration;
        while (clock::now() < timeout)
        {
            function(_prepared_iterations++);
        }
    }

private:
    void run_iteration_until(clock::time_point timeout)
    {
        while (clock::now() < timeout)
        {
            ((Derived *)this)->run_iteration(iterations());
            test_loop::add_iteration();
        }
    }

    void run_n_iterations(unsigned long count)
    {
        while (iterations() < count)
        {
            ((Derived *)this)->run_iteration(iterations());
            test_loop::add_iteration();
        }
    }

    test_config _config;
    unsigned long _prepared_iterations;
};

} // namespace tests
} // namespace bench
