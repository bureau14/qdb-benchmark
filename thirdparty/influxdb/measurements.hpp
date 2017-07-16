#pragma once

#include "measurement.hpp"
#include <string>

namespace idb
{
namespace api
{
namespace measurement
{
class measurements
{
public:
    measurements() = default;

    measurements(const measurements &) = default;
    measurements(measurements &&) = default;
    measurements &operator=(const measurements &) = default;
    measurements &operator=(measurements &&) = default;

    inline measurements & operator<<(const measurement & t);
    inline measurements & operator<<(measurement && t);

    inline const std::string & line() const;

private:
    std::string m_line;
};

measurements & measurements::operator<<(const measurement & m)
{
    if (!m_line.empty())
    {
        m_line += "\n";
    }
    m_line += m.line();
    return *this;
}
measurements & measurements::operator<<(measurement && m)
{
    if (!m_line.empty())
    {
        m_line += "\n";
    }
    m_line += m.line();
    return *this;
}

const std::string & measurements::line() const
{
    if (m_line.empty())
    {
        std::runtime_error(std::string("Measurements should have at least one measurement."));
    }
    return m_line;
}
}
}
}