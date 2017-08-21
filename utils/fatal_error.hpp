#pragma once

#include <stdexcept>
#include <string>

namespace utils
{
class fatal_error : public std::exception
{
public:
    fatal_error(const std::string & message) : _message("Fatal error: ")
    {
        _message += message;
    }

    const char * what() const throw()
    {
        return _message.c_str();
    }

private:
    std::string _message;
};
}
