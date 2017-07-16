#pragma once

#include <string>
#include <utility>

namespace idb
{
namespace api
{
namespace measurement
{
template <typename ValueType>
class field
{
public:
    field() = delete;
    field(const std::string & f, const ValueType & v) : m_key(f), m_value(v)
    {
    }
    field(std::string && f, ValueType && v) : m_key(std::move(f)), m_value(std::move(v))
    {
    }
    field(const field & f) : m_key(f.m_key), m_value(f.m_value)
    {
    }
    field(field && f) : m_key(std::move(f.m_key)), m_value(std::move(f.m_value))
    {
    }

    inline const std::string & key() const
    {
        return m_key;
    }

    inline const std::string value() const
    {
        return std::to_string(m_value);
    }

private:
    const std::string m_key;
    const ValueType m_value;
};

template <>
inline const std::string field<std::string>::value() const
{
    return std::string("\"") + m_value + std::string("\"");
}
}
}
}