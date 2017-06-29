#pragma once

#include <algorithm>
#include <iterator>
#include <iomanip>
#include <list>
#include <sstream>
#include <string>
#include <vector>

namespace utils
{

class command_line
{
    std::ostringstream _help;
    std::list<std::string> _args;

public:
    command_line(int argc, const char ** argv)
    {
        std::copy(argv + 1, argv + argc, std::inserter(_args, _args.end()));
    }

    std::string help()
    {
        return _help.str();
    }

    void check_unknown()
    {
        if (!_args.empty())
            throw std::runtime_error("Command line argument \"" + *_args.begin()
                                     + "\" is not supported");
    }

    bool get_flag(const std::string & short_syntax,
                  const std::string & long_syntax,
                  const std::string & description)
    {
        auto it = find(short_syntax, long_syntax, description, "");
        if (it == _args.end()) return false;
        _args.erase(it);
        return true;
    }

    std::string get_string(const std::string & short_syntax,
                           const std::string & long_syntax,
                           const std::string & description,
                           const std::string & default_value = "")
    {
        auto value = find(short_syntax, long_syntax, description, default_value);
        if (value == _args.end()) return default_value;
        auto flag = value++;
        if (value == _args.end()) return default_value;
        std::string result = *value;
        _args.erase(flag);
        _args.erase(value);
        return result;
    }

    std::vector<std::string> get_strings(const std::string & short_syntax,
                                         const std::string & long_syntax,
                                         const std::string & description,
                                         const std::string & default_value = "")
    {
        try
        {
            std::vector<std::string> values;
            std::string _args = get_string(short_syntax, long_syntax, description, default_value);

            for_each_token(_args, [&](const std::string & x) { values.push_back(x); });

            return values;
        }
        catch (...)
        {
            throw std::runtime_error("Command line argument " + long_syntax + " is invalid");
        }
    }

    int get_integer(const std::string & short_syntax,
                    const std::string & long_syntax,
                    const std::string & description,
                    const std::string & default_value)
    {
        try
        {
            return std::stoi(get_string(short_syntax, long_syntax, description, default_value));
        }
        catch (...)
        {
            throw std::runtime_error("Command line argument " + long_syntax + " is invalid");
        }
    }

    template <typename Value, typename Selector>
    void get_updatable_values(const std::string & short_syntax,
                              const std::string & long_syntax,
                              const std::string & description,
                              const std::string & default_value,
                              std::vector<Value> & values,
                              Selector selector)
    {
        std::string _args = get_string(short_syntax, long_syntax, description, default_value);

        for_each_token(_args, [&](const std::string & x) {
            // Selector may select many values.
            selector(x, values);
        });
    }

    template <typename Value, typename Selector>
    std::vector<Value> get_values(const std::string & short_syntax,
                                  const std::string & long_syntax,
                                  const std::string & description,
                                  const std::string & default_value,
                                  Selector selector)
    {
        std::vector<Value> values;
        std::string _args = get_string(short_syntax, long_syntax, description, default_value);

        for_each_token(_args, [&](const std::string & x) {
            // Selector may select many values.
            std::vector<Value> selected = selector(x);
            ::std::copy(selected.begin(), selected.end(), ::std::back_inserter(values));
        });

        return values;
    }

    std::vector<int> get_integers(const std::string & short_syntax,
                                  const std::string & long_syntax,
                                  const std::string & description,
                                  const std::string & default_value)
    {
        return get_values<int>(
            short_syntax, long_syntax, description, default_value,
            [](const std::string & s) { return std::vector<int>{std::stoi(s)}; });
    }

private:
    std::list<std::string>::iterator find(const std::string & short_syntax,
                                          const std::string & long_syntax,
                                          const std::string & description,
                                          const std::string & default_value)
    {
        _help << "  " << std::left << std::setw(3) << short_syntax << std::setw(25) << long_syntax << ' ';

        if (default_value.empty())
        {
            _help << std::setw(25) << ' ';
        }
        else
        {
            _help << std::setw(25) << (" (" + default_value + ") ");
        }

        _help << description;
        _help << std::endl;

        return std::find_if(_args.begin(), _args.end(), [&](const std::string & arg)
                            {
                                return arg == short_syntax || arg == long_syntax;
                            });
    }

    template <typename Function>
    static void for_each_token(const std::string & input, Function fn)
    {
        if (input.empty()) return;

        size_t start = 0;

        for (;;)
        {
            const size_t stop = input.find(',', start);

            std::string token = input.substr(start, stop - start);
            try
            {
                fn(token);
            }
            catch (...)
            {
                throw std::runtime_error("Unexpected token: " + token);
            }

            if (stop == std::string::npos) break;

            start = stop + 1u;
        }
    }
};
}
