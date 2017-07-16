#pragma once

#include "command.hpp"
#include <cpr/cpr.h>
#include <exception>
#include <iostream>
#include <memory>
#include <type_traits>

namespace idb
{
namespace api
{
class api
{
public:
    api() = default;
    api(const std::string & base_uri, const std::string & dbname) : m_base_uri(base_uri), m_dbname(dbname)
    {
    }
    api(const api & a) = default;
    api(api && a) = default;

    api & operator=(const api & a) = default;
    api & operator=(api && a) = default;

    template <typename QueryType>
    typename std::enable_if<std::is_base_of<command::command<QueryType>, QueryType>::value, void>::type
    execute(QueryType query)
    {
        typename QueryType::tag execution_tag;
        try
        {
            execute_impl(query, execution_tag);
        }
        catch (...)
        {
            std::rethrow_exception(std::current_exception());
        }
    }

    void createDatabase();
    void insert(const measurement::measurement & measurement);
    void insert(const measurement::measurements & measurements);

    void dropDatabase();
    void drop(const measurement::measurement & measurement);

    void select(const measurement::measurement & measurement);
    void select(const std::string & what, const std::string & from);
    void select(const std::string & what, const std::string & from, const std::string & where);

private:
    std::string m_base_uri;
    std::string m_dbname;

    template <typename QueryType>
    void execute_impl(QueryType query, command::execution_tag::get)
    {
        try
        {
            response_handler(query.request()->Get());
        }
        catch (...)
        {
            std::rethrow_exception(std::current_exception());
        }
    }

    template <typename QueryType>
    void execute_impl(QueryType query, command::execution_tag::post)
    {
        try
        {
            response_handler(query.request()->Post());
        }
        catch (...)
        {
            std::rethrow_exception(std::current_exception());
        }
    }

    void response_handler(cpr::Response response)
    {
        auto status = response.status_code;
        if (status != 200 && status != 204)
        {
            std::cout << "url: " << response.url << "\n";
            std::cout << "headers:\n";
            for (const auto & header : response.header)
            {
                std::cout << "\t" << header.first << ": " << header.second << "\n";
            }
            throw std::runtime_error(response.text);
        }
    }
};
}
}
