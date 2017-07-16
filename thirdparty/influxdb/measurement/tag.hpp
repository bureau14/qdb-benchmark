#pragma once

#include <string>
#include <utility>

namespace idb
{
namespace api
{
namespace measurement
{
class tag
{
public:
    tag() = delete;
    tag(const std::string & f, const std::string & v) : m_key(f), m_value(v)
    {
    }
    tag(std::string && f, std::string && v) : m_key(std::move(f)), m_value(std::move(v))
    {
    }
    tag(const tag & t) : m_key(t.m_key), m_value(t.m_value)
    {
    }
    tag(tag && t) : m_key(std::move(t.m_key)), m_value(std::move(t.m_value))
    {
    }

    inline const std::string & key() const
    {
        return m_key;
    }

    inline const std::string & value() const
    {
        return m_value;
    }

private:
    const std::string m_key;
    const std::string m_value;
};
}
}
}