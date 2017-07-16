#pragma once

#include "../command.hpp"

namespace idb
{
namespace api
{
namespace command
{
class query : public command<query>
{
public:
    query(const std::string & base_uri) : command(base_uri)
    {
    }
    inline void setPath()
    {
        m_url += "/query";
        m_request->SetOption(m_url);
    }
    inline void prepareStatement(const std::string & dbname, const std::string & statement)
    {
        auto parameters = cpr::Parameters{{"db", dbname}, {"q", statement}};
        m_request->SetOption(parameters);
    }
};
}
}
}