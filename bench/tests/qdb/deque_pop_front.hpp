#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>
#include <utils/random.hpp>

namespace bench
{
namespace tests
{
namespace qdb
{

class deque_pop_front : public qdb_test_template<deque_pop_front>
{
public:
    explicit deque_pop_front(bench::test_config config) : qdb_test_template(config)
    {
        _content = utils::create_random_string(config.content_size);
    }

    void setup() override
    {
        qdb_test_template::setup();

        setup_each([&](unsigned long iteration)
                   {
                       _qdb.deque_push_front(alias(0), _content);
                   });
    }

    void run_iteration(unsigned long iteration)
    {
        _qdb.deque_pop_front(alias(0));
    }

    static ::std::string name()
    {
        return "qdb_deque_pop_front";
    }

    static ::std::string description()
    {
        return "Each thread repeats qdb_deque_pop_front() on a queue";
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
