#include <bench/tests/qdb/quasardb_facade.hpp>
#include <utils/detailed_error.hpp>
#ifdef WIN32
#include <utils/win32.hpp>
#endif
#include <utils/invocation_string.hpp>

#include <qdb/blob.h>
#include <qdb/deque.h>
#include <qdb/hset.h>
#include <qdb/integer.h>
#include <qdb/stream.h>
#include <qdb/tag.h>

#include <cppformat/format.h>

#ifndef WIN32
#include <cstring>
#include <cerrno>
#endif

using namespace bench::tests::qdb;

#define ARG0(x, ...) x
#define STR0(x) #x
#define STR1(x) STR0(x)
#define INVOKE(...) named_invoke(STR1(ARG0(__VA_ARGS__, )), __VA_ARGS__)

using namespace bench::tests::qdb;

static bool is_error(qdb_error_t err)
{
    switch (err)
    {
    case qdb_e_ok:
    case qdb_e_ok_created:
    case qdb_e_element_already_exists:
    case qdb_e_element_not_found:
        return false;
    default:
        return true;
    }
}

template <typename Function, typename... Args>
static qdb_error_t named_invoke(const char * name, Function function, Args &&... args)
{
    qdb_error_t err = function(args...);
    if (is_error(err))
    {
        std::string message = qdb_error(err);
        std::string details =
            fmt::format("{} returned {:#08x}", utils::make_invocation_string(name, std::forward<Args>(args)...),
                        std::uint32_t(err));

        if ((err == qdb_e_system_local) || (err == qdb_e_system_remote))
        {
#ifdef WIN32
            std::uint32_t lastError = utils::win32::get_last_error();
            message = utils::win32::get_error_string(lastError);
            details += fmt::format(" and GetLastError() returned {}", lastError);
#else
            message = std::strerror(errno);
            details += fmt::format(" and errno equals {}", errno);
#endif
        }

        throw utils::detailed_error(message, details);
    }
    return err;
}

quasardb_facade::quasardb_facade()
{
    _handle = qdb_open_tcp();
}

quasardb_facade::~quasardb_facade()
{
    qdb_close(_handle);
}

void quasardb_facade::connect(const std::string & cluster_uri)
{
    INVOKE(qdb_connect, _handle, cluster_uri.c_str());
}

void quasardb_facade::close()
{
    INVOKE(qdb_close, _handle);
}

void quasardb_facade::trim_all(int timeout)
{
    INVOKE(qdb_trim_all, _handle, timeout);
}

void quasardb_facade::free_buffer(const char * buffer)
{
    qdb_free_buffer(_handle, buffer);
}

std::string quasardb_facade::node_status(const std::string & node_uri) const
{
    const char * content;
    size_t content_size;
    INVOKE(qdb_node_status, _handle, node_uri.c_str(), &content, &content_size);
    std::string json(content, content_size);
    qdb_free_buffer(_handle, content);
    return json;
}

std::string quasardb_facade::node_topology(const std::string & node_uri) const
{
    const char * content;
    size_t content_size;
    INVOKE(qdb_node_topology, _handle, node_uri.c_str(), &content, &content_size);
    std::string json(content, content_size);
    qdb_free_buffer(_handle, content);
    return json;
}

void quasardb_facade::blob_put(const std::string & alias, const std::string & content)
{
    INVOKE(qdb_blob_put, _handle, alias.c_str(), content.data(), content.size(), qdb_never_expires);
}

bool quasardb_facade::blob_update(const std::string & alias, const std::string & content)
{
    return INVOKE(qdb_blob_update, _handle, alias.c_str(), content.data(), content.size(), qdb_preserve_expiration)
           == qdb_e_ok_created;
}

qdb_buffer quasardb_facade::blob_get(const std::string & alias)
{
    const void * result;
    std::size_t result_size;
    INVOKE(qdb_blob_get, _handle, alias.c_str(), &result, &result_size);
    return qdb_buffer(_handle, result, result_size);
}

void quasardb_facade::blob_get_noalloc(const std::string & alias, std::string & content)
{
    std::size_t result_size = content.capacity();
    INVOKE(qdb_blob_get_noalloc, _handle, alias.c_str(), const_cast<char *>(content.data()), &result_size);
    content.resize(result_size);
}

void quasardb_facade::remove(const std::string & alias)
{
    INVOKE(qdb_remove, _handle, alias.c_str());
}

qdb_buffer quasardb_facade::deque_pop_back(const std::string & alias)
{
    const void * result;
    std::size_t result_size;
    INVOKE(qdb_deque_pop_back, _handle, alias.c_str(), &result, &result_size);
    return qdb_buffer(_handle, result, result_size);
}

qdb_buffer quasardb_facade::deque_pop_front(const std::string & alias)
{
    const void * result;
    std::size_t result_size;
    INVOKE(qdb_deque_pop_front, _handle, alias.c_str(), &result, &result_size);
    return qdb_buffer(_handle, result, result_size);
}

void quasardb_facade::deque_push_back(const std::string & alias, const std::string & content)
{
    INVOKE(qdb_deque_push_back, _handle, alias.c_str(), content.data(), content.size());
}

void quasardb_facade::deque_push_front(const std::string & alias, const std::string & content)
{
    INVOKE(qdb_deque_push_front, _handle, alias.c_str(), content.data(), content.size());
}

std::int64_t quasardb_facade::int_add(const std::string & alias, std::int64_t value)
{
    std::int64_t total;
    INVOKE(qdb_int_add, _handle, alias.c_str(), 1, &total);
    return total;
}

void quasardb_facade::int_put(const std::string & alias, std::int64_t value)
{
    INVOKE(qdb_int_put, _handle, alias.c_str(), value, qdb_never_expires);
}

bool quasardb_facade::int_update(const std::string & alias, std::int64_t value)
{
    return INVOKE(qdb_int_update, _handle, alias.c_str(), value, qdb_preserve_expiration) == qdb_e_ok_created;
}

std::int64_t quasardb_facade::int_get(const std::string & alias)
{
    std::int64_t value;
    INVOKE(qdb_int_get, _handle, alias.c_str(), &value);
    return value;
}

bool quasardb_facade::hset_contains(const std::string & alias, const std::string & content)
{
    return INVOKE(qdb_hset_contains, _handle, alias.c_str(), content.data(), content.size())
           == qdb_e_element_already_exists;
}

bool quasardb_facade::hset_erase(const std::string & alias, const std::string & content)
{
    return INVOKE(qdb_hset_contains, _handle, alias.c_str(), content.data(), content.size()) != qdb_e_element_not_found;
}

bool quasardb_facade::hset_insert(const std::string & alias, const std::string & content)
{
    return INVOKE(qdb_hset_insert, _handle, alias.c_str(), content.data(), content.size())
           != qdb_e_element_already_exists;
}

void quasardb_facade::add_tag(const std::string & alias, const std::string & tag)
{
    INVOKE(qdb_add_tag, _handle, alias.c_str(), tag.c_str());
}

void quasardb_facade::has_tag(const std::string & alias, const std::string & tag)
{
    INVOKE(qdb_has_tag, _handle, alias.c_str(), tag.c_str());
}

void quasardb_facade::remove_tag(const std::string & alias, const std::string & tag)
{
    INVOKE(qdb_remove_tag, _handle, alias.c_str(), tag.c_str());
}

qdb_stream_t quasardb_facade::stream_open(const std::string & alias, qdb_stream_mode_t mode)
{
    qdb_stream_t stream;
    INVOKE(qdb_stream_open, _handle, alias.c_str(), mode, &stream);
    return stream;
}

void qdb_stream_facade::close()
{
    INVOKE(qdb_stream_close, _handle);
}

void qdb_stream_facade::write(const std::string & content)
{
    INVOKE(qdb_stream_write, _handle, content.data(), content.size());
}
