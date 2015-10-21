#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>
#include <utils/random.hpp>

namespace bench
{
namespace tests
{
namespace qdb
{

class blob_remove : public qdb_test_template<blob_remove>
{
public:
    explicit blob_remove(bench::test_config config) : qdb_test_template(config)
    {
        _content = utils::create_random_string(config.content_size);
    }

    void setup() override
    {
        qdb_test_template::setup();

        setup_each([=](unsigned long iteration)
                   {
                       _qdb.blob_put(alias(iteration), _content);
                   });
    }

    void run_iteration(unsigned long iteration)
    {
        _qdb.remove(alias(iteration));
    }

    static ::std::string name()
    {
        return "qdb_blob_remove";
    }

    static ::std::string description()
    {
        return "Each thread repeats qdb_remove() on blob entries";
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
