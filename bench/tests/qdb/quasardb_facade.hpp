#pragma once

#include <qdb/client.h>
#include <qdb/stream.h>
#include <qdb/ts.h>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

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
        if (_content) qdb_release(_handle, _content);
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
    void set_cluster_security(const std::string & cluster_public_file);
    void set_user_security(const std::string & user_credentials_file);
    void trim_all(int timeout);

    std::string node_status(const std::string & node_uri) const;
    std::string node_topology(const std::string & node_uri) const;
    void free_buffer(const char * buffer);

    void remove(const std::string & alias);

    void blob_put(const std::string & alias, const std::string & content);
    bool blob_update(const std::string & alias, const std::string & content);
    qdb_buffer blob_get(const std::string & alias);
    void blob_get_noalloc(const std::string & alias, std::string & content);

    qdb_buffer deque_get_at(const std::string & alias, qdb_int_t index);
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

    void attach_tag(const std::string & alias, const std::string & tag);
    void has_tag(const std::string & alias, const std::string & tag);
    void detach_tag(const std::string & alias, const std::string & tag);
    void get_tagged(const std::string & tag);
    void get_tags(const std::string & alias);

    void ts_create(const std::string & alias, const std::vector<qdb_ts_column_info_t> & columns);
    void ts_col_blob_insert(const std::string & alias,
                            const std::string & col_name,
                            const qdb_timespec_t & ts,
                            const std::string & content);
    void ts_col_double_insert(const std::string & alias,
                              const std::string & col_name,
                              const qdb_timespec_t & ts,
                              double content);
    void ts_col_double_inserts(const std::string & alias,
                               const std::string & col_name,
                               const qdb_ts_double_point * points,
                               size_t count);
    void ts_col_double_average(const std::string & alias, const std::string & col_name, const qdb_ts_range_t & range);

    qdb_stream_t stream_open(const std::string & alias, qdb_stream_mode_t mode);

private:
    qdb_handle_t _handle;
};

} // namespace qdb
} // namespace tests
} // namespace bench
