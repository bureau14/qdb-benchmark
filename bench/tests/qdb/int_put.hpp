#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>

namespace bench
{
namespace tests
{
namespace qdb
{

class int_put : public qdb_test_template<int_put>
{
public:
    explicit int_put(bench::test_config config) : qdb_test_template(config)
    {
    }

    void run_iteration(unsigned long iteration)
    {
        _qdb.int_put(alias(iteration), 42);
    }

    void cleanup() override
    {
        cleanup_each([&](unsigned long iteration)
                     {
                         _qdb.remove(alias(iteration));
                     });
    }

    static ::std::string name()
    {
        return "qdb_int_put";
    }

    static ::std::string description()
    {
        return "Each thread repeats qdb_put() with new aliases";
    }

    static bool size_dependent()
    {
        return false;
    }
};

} // namespace qdb
} // namespace tests
} // namespace bench
