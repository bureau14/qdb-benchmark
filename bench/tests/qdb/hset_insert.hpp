#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>
#include <utils/random.hpp>

namespace bench
{
namespace tests
{
namespace qdb
{

class hset_insert : public qdb_test_template<hset_insert>
{
public:
    explicit hset_insert(bench::test_config config) : qdb_test_template(config)
    {
        _content = utils::create_random_string(config.content_size);
    }

    void run_iteration(unsigned long iteration)
    {
        set_watermark(_content, iteration);
        _qdb.hset_insert(alias(0), _content);
    }

    void cleanup() override
    {
        _qdb.remove(alias(0));
    }

    static ::std::string name()
    {
        return "qdb_hset_insert";
    }

    static ::std::string description()
    {
        return "Call qdb_hset_insert() on one entry";
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
