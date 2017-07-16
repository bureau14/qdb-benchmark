#pragma once

#include "measurement/field.hpp"
#include "measurement/tag.hpp"
#include <memory>
#include <stdexcept>
#include <string>

#ifdef _MSC_VER
typedef __time64_t idb_time_t;
#else
#include <stdint.h>
typedef time_t idb_time_t;
#endif

namespace idb
{
namespace api
{
namespace measurement
{
class measurement
{
    enum class WritingState
    {
        name,
        tags,
        fields,
        timestamp
    };

public:
    measurement() = delete;
    measurement(const std::string & name) : m_state(WritingState::name), m_name(name)
    {
        m_line += m_name;
    }

    measurement(const measurement & rhs) = default;
    measurement(measurement && rhs) = default;

    measurement & operator=(const measurement &) = default;
    measurement & operator=(measurement &&) = default;

    measurement & operator<<(const tag & t);
    measurement & operator<<(tag && t);

    template <typename FieldType>
    measurement & operator<<(const field<FieldType> & f);
    template <typename FieldType>
    measurement & operator<<(field<FieldType> && f);

    measurement & operator<<(const idb_time_t & t);
    measurement & operator<<(idb_time_t && t);

    inline const std::string & line() const;

    inline const std::string & name() const;

private:
    WritingState m_state;
    std::string m_line;
    const std::string m_name;
};

template <typename FieldType>
measurement & measurement::operator<<(const field<FieldType> & f)
{
    if (m_state == WritingState::timestamp)
    {
        std::runtime_error(std::string("Measurement should not write fields after timestamp."));
    }
    if (m_state != WritingState::fields)
    {
        m_line += " ";
    }
    else
    {
        m_line += ",";
    }
    m_line += f.key();
    m_line += "=";
    m_line += f.value();
    m_state = WritingState::fields;
    return *this;
}
template <typename FieldType>
measurement & measurement::operator<<(field<FieldType> && f)
{
    if (m_state == WritingState::timestamp)
    {
        std::runtime_error(std::string("Measurement should not write fields after timestamp."));
    }
    if (m_state != WritingState::fields)
    {
        m_line += " ";
    }
    else
    {
        m_line += ",";
    }
    m_line += std::move(f.key());
    m_line += "=";
    m_line += f.value();
    m_state = WritingState::fields;
    return *this;
}

const std::string & measurement::line() const
{
    return m_line;
}
const std::string & measurement::name() const
{
    return m_name;
}
}
}
}