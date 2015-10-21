#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>
#include <utils/random.hpp>

namespace bench
{
namespace tests
{
namespace qdb
{

class blob_get : public qdb_test_template<blob_get>
{
public:
    blob_get(bench::test_config config) : qdb_test_template(config)
    {
        _content = utils::create_random_string(config.content_size);
    }

    void setup() override
    {
        qdb_test_template::setup();
        _qdb.blob_put(alias(0), _content);
    }

    void run_iteration(unsigned long iteration)
    {
        auto content = _qdb.blob_get(alias(0));
        if (content.size() != _content.size()) throw ::std::exception();
    }

    void cleanup() override
    {
        _qdb.remove(alias(0));
    }

    static ::std::string name()
    {
        return "qdb_blob_get";
    }

    static ::std::string description()
    {
        return "Each thread repeats qdb_blob_get() on one entry";
    }

    static bool size_dependent()
    {
        return true;
    }

private:
    ::std::string _content;
};

} // namespace qdb
} // namespace tests
} // namespace bench
