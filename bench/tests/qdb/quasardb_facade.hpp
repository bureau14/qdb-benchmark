#pragma once

#include <qdb/client.h>
#include <qdb/stream.h>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace bench
{
namespace tests
{
namespace qdb
{
class qdb_buffer
{
public:
    qdb_buffer() : _handle(nullptr), _content(nullptr), _content_size(0)
    {
    }

    qdb_buffer(qdb_handle_t h, const void * content, size_t content_size)
        : _handle(h), _content(content), _content_size(content_size)
    {
    }

    qdb_buffer(const qdb_buffer &) = delete;
    qdb_buffer(qdb_buffer && other) : qdb_buffer()
    {
        std::swap(_handle, other._handle);
        std::swap(_content, other._content);
        std::swap(_content_size, other._content_size);
    }

    ~qdb_buffer()
    {
        if (_content) qdb_free_buffer(_handle, _content);
    }

    const void * data() const
    {
        return _content;
    }

    size_t size() const
    {
        return _content_size;
    }

private:
    qdb_handle_t _handle;
    const void * _content;
    size_t _content_size;
};

class qdb_stream_facade
{
public:
    qdb_stream_facade(qdb_stream_t h = nullptr) : _handle(h)
    {
    }

    void close();
    void write(const std::string & content);

private:
    qdb_stream_t _handle;
};

class quasardb_facade
{
public:
    quasardb_facade();
    ~quasardb_facade();

    void connect(const std::string & cluster_uri);
    void close();
    void trim_all(int timeout);

    std::string node_status(const std::string & node_uri) const;
    std::string node_topology(const std::string & node_uri) const;
    void free_buffer(const char * buffer);

    void remove(const std::string & alias);

    void blob_put(const std::string & alias, const std::string & content);
    bool blob_update(const std::string & alias, const std::string & content);
    qdb_buffer blob_get(const std::string & alias);
    void blob_get_noalloc(const std::string & alias, std::string & content);

    qdb_buffer deque_pop_back(const std::string & alias);
    qdb_buffer deque_pop_front(const std::string & alias);
    void deque_push_back(const std::string & alias, const std::string & content);
    void deque_push_front(const std::string & alias, const std::string & content);

    std::int64_t int_add(const std::string & alias, std::int64_t value);
    void int_put(const std::string & alias, std::int64_t value);
    bool int_update(const std::string & alias, std::int64_t value);
    std::int64_t int_get(const std::string & alias);

    bool hset_contains(const std::string & alias, const std::string & content);
    bool hset_erase(const std::string & alias, const std::string & content);
    bool hset_insert(const std::string & alias, const std::string & content);

    void add_tag(const std::string & alias, const std::string & tag);
    void has_tag(const std::string & alias, const std::string & tag);
    void remove_tag(const std::string & alias, const std::string & tag);

    qdb_stream_t stream_open(const std::string & alias, qdb_stream_mode_t mode);

private:
    qdb_handle_t _handle;
};

} // namespace qdb
} // namespace tests
} // namespace bench
