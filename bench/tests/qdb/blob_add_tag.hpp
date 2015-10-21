#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>
#include <utils/random.hpp>

namespace bench
{
namespace tests
{
namespace qdb
{

class blob_add_tag : public qdb_test_template<blob_add_tag>
{
public:
    blob_add_tag(bench::test_config config) : qdb_test_template(config)
    {
        _target_alias = alias(0) + "-target";
    }

    void setup() override
    {
        qdb_test_template::setup();
        _qdb.blob_put(_target_alias, utils::create_random_string(16));
    }

    void run_iteration(unsigned long iteration)
    {
        _qdb.add_tag(_target_alias, alias(iteration));
    }

    void cleanup() override
    {
        _qdb.remove(_target_alias);

        cleanup_each([=](unsigned long iteration)
                     {
                         _qdb.remove(alias(iteration));
                     });
    }

    static ::std::string name()
    {
        return "qdb_blob_add_tag";
    }

    static ::std::string description()
    {
        return "Each thread repeats qdb_add_tag() on one blob";
    }

    static bool size_dependent()
    {
        return false;
    }

private:
    ::std::string _target_alias;
};

} // namespace qdb
} // namespace tests
} // namespace bench
