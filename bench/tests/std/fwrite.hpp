#pragma once

#include <bench/tests/std/file_exception.hpp>

#include <cstdio>
#include <string>

namespace bench
{
namespace tests
{
namespace std
{

class fwrite : public test_template<fwrite>
{
public:
    fwrite(bench::test_config config) : test_template(config)
    {
        _filename = utils::get_temporary_filename();
        _content = utils::create_random_string(config.content_size);
    }

    void run_iteration(unsigned long iteration)
    {
        auto fp = ::std::fopen(_filename.c_str(), "wb");
        if (fp == nullptr) throw create_file_exception(_filename, errno);

        ::std::fwrite(const_cast<char *>(_content.c_str()), _content.size(), 1, fp);
        auto err = ::std::ferror(fp);

        ::std::fclose(fp);

        if (err != 0) throw write_file_exception(_filename, err);
    }

    void cleanup() override
    {
        ::std::remove(_filename.c_str());
    }

    static ::std::string name()
    {
        return "std_fwrite";
    }

    static ::std::string description()
    {
        return "Call fwrite() on one file.";
    }

    static bool size_dependent()
    {
        return true;
    }

private:
    ::std::string _filename;
    ::std::string _content;
};

} // namespace std
} // namespace tests
} // namespace bench