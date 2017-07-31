#pragma once

#include <bench/core/test_instance.hpp>
#include <fstream>

namespace bench
{
namespace report
{

class jsonp
{
public:
    jsonp()
    {
        _file.open("results.jsonp");
    }

    jsonp(const std::string & filename)
    {
        _file.open(filename);
    }

    ~jsonp()
    {
        _file.close();
    }

    void add_test(const test_instance & test);
    void write_synthesis(const test_instance_collection & test);

private:
    std::ofstream _file;
};

} // namespace report
} // namespace bench
