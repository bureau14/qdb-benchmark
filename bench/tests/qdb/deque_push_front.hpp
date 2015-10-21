#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>
#include <utils/random.hpp>

namespace bench
{
namespace tests
{
namespace qdb
{

class deque_push_front : public qdb_test_template<deque_push_front>
{
public:
    explicit deque_push_front(bench::test_config config) : qdb_test_template(config)
    {
        _content = utils::create_random_string(config.content_size);
    }

    void run_iteration(unsigned long iteration)
    {
        _qdb.deque_push_front(alias(0), _content);
    }

    void cleanup() override
    {
        _qdb.remove(alias(0));
    }

    static ::std::string name()
    {
        return "qdb_deque_push_front";
    }

    static ::std::string description()
    {
        return "Each thread repeats qdb_deque_push_front() on a queue";
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
