#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>

#include <qdb/integer.h>

namespace bench
{
namespace tests
{
namespace qdb
{

class int_add : public qdb_test_template<int_add>
{
public:
    explicit int_add(bench::test_config config) : qdb_test_template(config)
    {
    }

    void setup() override
    {
        qdb_test_template::setup();
        _qdb.int_put(alias(0), 0);
    }

    void run_iteration(unsigned long iteration)
    {
        _qdb.int_add(alias(0), 1);
    }

    void cleanup() override
    {
        _qdb.remove(alias(0));
    }

    static ::std::string name()
    {
        return "qdb_int_add";
    }

    static ::std::string description()
    {
        return "Each thread repeats qdb_int_add() on one entry";
    }

    static bool size_dependent()
    {
        return false;
    }
};

} // namespace qdb
} // namespace tests
} // namespace bench
