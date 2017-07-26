#pragma once

#include <bench/app/settings.hpp>
#include <bench/core/test_class.hpp>
#include <boost/any.hpp>
#include <boost/program_options.hpp>

namespace bench
{
namespace app
{
class options
{
    using description = boost::program_options::options_description;
    using variables_map = boost::program_options::variables_map;

public:
    options(test_class_collection & test_pool) : _desc("Options"), _test_pool(test_pool)
    {
        init();
    }

    // parse entry - exit on fail
    void parse(int argc, const char ** argv);
    void fill_settings(settings & settings);

private:
    description _desc;
    variables_map _vmap;
    test_class_collection & _test_pool;

    void init();
    test_class_collection parse_tests();
    template <typename T>
    T get_value(const std::string & name)
    {
        return _vmap[name].as<T>();
    }
};
} // namespace app
} // namespace bench
