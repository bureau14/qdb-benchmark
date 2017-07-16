#pragma once

#include "measurement.hpp"
#include "measurements.hpp"
#include <cpr/cpr.h>
#include <memory>
#include <utility>

namespace idb
{
namespace api
{
namespace command
{
struct execution_tag
{
    struct get
    {
    };
    struct post
    {
    };

    // doesn't really make sense in this use case
    // since this is a time series database
    // struct put {};
};
template <typename Derived>
class command
{
    using Payload = std::pair<std::string, std::string>;

public:
    using tag = execution_tag::get;

    command(const std::string & base_uri) : m_request(std::make_shared<cpr::Session>())
    {
        m_url += base_uri;
        static_cast<Derived *>(this)->setPath();
    }

    template <typename... Args>
    void prepare(Args... args)
    {
        static_cast<Derived *>(this)->prepareStatement(args...);
    }

    std::shared_ptr<cpr::Session> request() const
    {
        return m_request;
    }

protected:
    std::shared_ptr<cpr::Session> m_request;
    cpr::Url m_url;
};
}
}
}