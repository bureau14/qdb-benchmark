#pragma once

#include "../command.hpp"

namespace idb
{
namespace api
{
namespace command
{
class drop : public command<drop>
{
public:
    using tag = execution_tag::post;
    drop(const std::string & base_uri) : command(base_uri)
    {
    }
    inline void setPath()
    {
        m_url += "/query";
        m_request->SetOption(m_url);
    }
    inline void prepareStatement(const std::string & dbname)
    {
        auto parameters = cpr::Parameters{{"q", "DROP DATABASE \"" + dbname + "\""}};
        m_request->SetOption(parameters);
    }
};
}
}
}