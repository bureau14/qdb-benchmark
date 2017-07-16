#include "api.hpp"
#include "command/create.hpp"
#include "command/drop.hpp"
#include "command/insert.hpp"
#include "command/query.hpp"
#include <cppformat/format.h>
#include <iomanip>

void idb::api::api::createDatabase()
{
    auto create = command::create(m_base_uri);
    create.prepare(m_dbname);
    try
    {
        execute(create);
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}

void idb::api::api::insert(const measurement::measurement & mes)
{
    auto create = command::insert(m_base_uri);
    create.prepare(m_dbname, mes);
    try
    {
        execute(create);
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}

void idb::api::api::insert(const measurement::measurements & mes)
{
    auto create = command::insert(m_base_uri);
    create.prepare(m_dbname, mes);
    try
    {
        execute(create);
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}

void idb::api::api::dropDatabase()
{
    auto drop = command::drop(m_base_uri);
    drop.prepare(m_dbname);
    try
    {
        execute(drop);
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}

void idb::api::api::drop(const measurement::measurement & measurement)
{
    auto drop = command::query(m_base_uri);
    std::string statement = fmt::format("DROP MEASUREMENT \"{}\"", measurement.name());
    drop.prepare(m_dbname, statement);
    try
    {
        execute(drop);
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}

void idb::api::api::select(const measurement::measurement & measurement)
{
    auto select = command::query(m_base_uri);
    std::string statement = fmt::format("SELECT * FROM \"{}\"", measurement.name());
    select.prepare(m_dbname, statement);
    try
    {
        execute(select);
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}

void idb::api::api::select(const std::string & what, const std::string & from)
{
    auto select = command::query(m_base_uri);
    std::string statement = fmt::format("SELECT {} FROM {}", what, from);
    select.prepare(m_dbname, statement);
    try
    {
        execute(select);
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}

void idb::api::api::select(const std::string & what, const std::string & from, const std::string & where)
{
    auto select = command::query(m_base_uri);
    std::string statement = fmt::format("SELECT {} FROM {} WHERE {}", what, from, where);
    select.prepare(m_dbname, statement);
    try
    {
        execute(select);
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}