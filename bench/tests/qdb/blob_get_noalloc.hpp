#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>
#include <utils/random.hpp>

namespace bench
{
namespace tests
{
namespace qdb
{

class blob_get_noalloc : public qdb_test_template<blob_get_noalloc>
{
public:
    blob_get_noalloc(bench::test_config config)
        : qdb_test_template(config), _buffer(config.content_size, 0)
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
        _qdb.blob_get_noalloc(alias(0), _buffer);
        if (_content.size() != _buffer.size()) throw ::std::exception();
    }

    void cleanup() override
    {
        _qdb.remove(alias(0));
    }

    static ::std::string name()
    {
        return "qdb_blob_get_noalloc";
    }

    static ::std::string description()
    {
        return "Each thread repeats qdb_blob_get_noalloc() on one entry";
    }

    static bool size_dependent()
    {
        return true;
    }

private:
    ::std::string _buffer;
    ::std::string _content;
};

} // namespace qdb
} // namespace tests
} // namespace bench
