#pragma once

#include <stdexcept>
#include <string>

namespace bench
{
namespace tests
{
namespace std
{

class file_exception : public ::std::exception
{
public:
    file_exception(const ::std::string & action, const ::std::string & filename, int error)
    {
        _message =
            "Failed to " + action + " " + filename + " (error " + ::std::to_string(error) + ")";
    }

    virtual const char * what() const throw()
    {
        return _message.c_str();
    }

private:
    ::std::string _message;
};

class create_file_exception : public file_exception
{
public:
    create_file_exception(const ::std::string & filename, int error)
        : file_exception("create", filename, error)
    {
    }
};

class open_file_exception : public file_exception
{
public:
    open_file_exception(const ::std::string & filename, int error)
        : file_exception("open", filename, error)
    {
    }
};

class read_file_exception : public file_exception
{
public:
    read_file_exception(const ::std::string & filename, int error)
        : file_exception("read to", filename, error)
    {
    }
};

class write_file_exception : public file_exception
{
public:
    write_file_exception(const ::std::string & filename, int error)
        : file_exception("write to", filename, error)
    {
    }
};

} // namespace std
} // namespace tests
} // namespace bench