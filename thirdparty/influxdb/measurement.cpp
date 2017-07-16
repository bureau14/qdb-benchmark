#include "measurement.hpp"

namespace idb
{
namespace api
{
namespace measurement
{
measurement & measurement::operator<<(const tag & t)
{
    if (m_state == WritingState::fields)
    {
        std::runtime_error(std::string("Measurement should not write tags after fields."));
    }
    else if (m_state == WritingState::timestamp)
    {
        std::runtime_error(std::string("Measurement should not write tags after timestamp."));
    }
    m_line += ",";
    m_line += t.key();
    m_line += "=";
    m_line += t.value();
    m_state = WritingState::tags;
    return *this;
}
measurement & measurement::operator<<(tag && t)
{
    if (m_state == WritingState::fields)
    {
        std::runtime_error(std::string("Measurement should not write tags after fields."));
    }
    else if (m_state == WritingState::timestamp)
    {
        std::runtime_error(std::string("Measurement should not write tags after timestamp."));
    }
    m_line += ",";
    m_line += std::move(t.key());
    m_line += "=";
    m_line += std::move(t.value());
    m_state = WritingState::tags;
    return *this;
}
measurement & measurement::operator<<(const idb_time_t & t)
{
    if (m_state == WritingState::timestamp)
    {
        std::runtime_error(std::string("Measurement should have none or only one timestamp."));
    }
    else if (m_state != WritingState::fields)
    {
        std::runtime_error(std::string("Measurement should have at least one field."));
    }
    m_line += " ";
    m_line += std::to_string(t);
    m_state = WritingState::timestamp;
    return *this;
}
measurement & measurement::operator<<(idb_time_t && t)
{
    if (m_state == WritingState::timestamp)
    {
        std::runtime_error(std::string("Measurement should have none or only one timestamp."));
    }
    else if (m_state != WritingState::fields)
    {
        std::runtime_error(std::string("Measurement should have at least one field."));
    }
    m_line += " ";
    m_line += std::to_string(std::move(t));
    m_state = WritingState::timestamp;
    return *this;
}
}
}
}
