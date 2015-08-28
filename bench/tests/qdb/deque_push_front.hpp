#pragma once

#include <bench/tests/qdb/test_base.hpp>
#include <utils/random.hpp>

namespace bench
{
namespace tests
{
namespace qdb
{
class deque_push_front : public test_base<deque_push_front>
{
public:
    explicit deque_push_front(bench::test_config config) : test_base(config)
    {
        _content = utils::create_random_string(config.content_size);
    }

    void run_iteration(unsigned long iteration)
    {
        _qdb.deque_push_front(_alias, _content);
    }

    ~deque_push_front()
    {
        _qdb.remove(_alias);
    }

    static std::string name()
    {
        return "qdb_deque_push_front";
    }

    static std::string description()
    {
        return "Call qdb_deque_push_front() on one entry.";
    }

    static bool size_dependent()
    {
        return true;
    }

private:
    std::string _content;
};
}
}
}
