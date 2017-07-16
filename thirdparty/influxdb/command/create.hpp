#pragma once

#include "../command.hpp"

namespace idb
{
namespace api
{
namespace command
{
class create : public command<create>
{
public:
    using tag = execution_tag::post;
    create(const std::string & base_uri) : command(base_uri)
    {
    }
    inline void setPath()
    {
        m_url += "/query";
        m_request->SetOption(m_url);
    }
    inline void prepareStatement(const std::string & dbname)
    {
        auto parameters = cpr::Parameters{{"q", "CREATE DATABASE \"" + dbname + "\""}};
        m_request->SetOption(parameters);
    }
};
}
}
}