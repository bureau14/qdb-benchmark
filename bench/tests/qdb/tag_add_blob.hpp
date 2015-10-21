#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>
#include <utils/random.hpp>

namespace bench
{
namespace tests
{
namespace qdb
{
class tag_add_blob : public qdb_test_template<tag_add_blob>
{
public:
    tag_add_blob(bench::test_config config) : qdb_test_template(config)
    {
        _tag = alias(0) + "-tag";
        _content = utils::create_random_string(16);
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
        _qdb.add_tag(alias(iteration), _tag);
    }

    void cleanup() override
    {
        _qdb.remove(_tag);
        cleanup_each([=](unsigned long iteration)
                     {
                         _qdb.remove(alias(iteration));
                     });
    }

    static ::std::string name()
    {
        return "qdb_tag_add_blob";
    }

    static ::std::string description()
    {
        return "Each thread repeats qdb_add_tag() on many blobs";
    }

    static bool size_dependent()
    {
        return false;
    }

private:
    ::std::string _tag;
    ::std::string _content;
};

} // namespace qdb
} // namespace tests
} // namespace bench
