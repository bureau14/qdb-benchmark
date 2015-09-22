#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>
#include <utils/random.hpp>

#include <qdb/blob.h>

namespace bench
{
namespace tests
{
namespace qdb
{
class blob_update : public qdb_test_template<blob_update>
{
public:
    explicit blob_update(bench::test_config config) : qdb_test_template(config)
    {
        _alias = get_alias(0); // only one alias is needed
        _content = utils::create_random_string(config.content_size);
    }

    void run_iteration(unsigned long iteration)
    {
        _qdb.blob_update(_alias, _content);
    }

    void cleanup() override
    {
        _qdb.remove(_alias);
    }

    static std::string name()
    {
        return "qdb_blob_update";
    }

    static std::string description()
    {
        return "Call qdb_update() on one entry.";
    }

    static bool size_dependent()
    {
        return true;
    }

private:
    std::string _alias;
    std::string _content;
};
}
}
}
