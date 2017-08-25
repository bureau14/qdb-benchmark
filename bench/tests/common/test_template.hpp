#pragma once

#include <bench/core/test_class.hpp>
#include <utils/detailed_error.hpp>
#include <utils/memory.hpp>
#include <chrono>

namespace bench
{
namespace tests
{

template <typename Derived> // CRTP
class test_template : public test_loop
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

        std::unique_ptr<test_loop> create_loop(test_config config) const override
        {
            return utils::make_unique<Derived>(config);
        }

        probe_collection create_probes(test_config config) const override
        {
            return Derived::create_probes(config);
        }
    };

    static probe_collection create_probes(test_config config)
    {
        return {};
    }

    test_template(test_config config) : _config(config), _prepared_iterations(config.iterations)
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
        for (auto i = 0u; i < test_loop::iterations(); i++)
        {
            function(i);
        }
    }

    template <typename Function>
    void setup_each(Function function)
    {
        if (_prepared_iterations != 0)
        {
            for (auto iteration = std::size_t(0); iteration < _prepared_iterations; ++iteration)
            {
                function(iteration);
            }
        }
        else
        {
            clock::time_point timeout = clock::now() + _config.duration;
            while (clock::now() < timeout)
            {
                function(_prepared_iterations++);
            }
        }
    }

private:
    void run_iteration_until(clock::time_point timeout)
    {
        while (clock::now() < timeout)
        {
            iterate();
        }
    }

    void run_n_iterations(std::uint32_t count)
    {
        while (iterations() < count)
        {
            iterate();
        }
    }

    void iterate()
    {
        try
        {
            auto start = std::chrono::high_resolution_clock::now();
            static_cast<Derived *>(this)->run_iteration(iterations());
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<std::uint64_t, std::nano> duration = end - start;
            test_loop::add_duration(duration.count());
        }
        catch (utils::detailed_error & e)
        {
            test_loop::add_failure();
        }
        catch (std::exception & e)
        {
            test_loop::add_failure();
        }
        test_loop::add_iteration();
    }

    test_config _config;
    std::uint32_t _prepared_iterations;
};

} // namespace tests
} // namespace bench
