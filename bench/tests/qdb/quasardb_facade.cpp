#include <bench/tests/qdb/quasardb_facade.hpp>
#include <utils/fatal_error.hpp>
#include <utils/invocation_string.hpp>
#include <ArduinoJson.h>
#include <fstream>
#ifdef _WIN32
#include <utils/win32.hpp>
#endif
#include <qdb/blob.h>
#include <qdb/deque.h>
#include <qdb/hset.h>
#include <qdb/integer.h>
#include <qdb/node.h>
#include <qdb/stream.h>
#include <qdb/tag.h>
#include <qdb/ts.h>
#include <cppformat/format.h>
#ifndef _WIN32
#include <cerrno>
#include <cstring>
#endif

using namespace bench::tests::qdb;

#define ARG0(x, ...) x
#define STR0(x) #x
#define STR1(x) STR0(x)
#define INVOKE(...) named_invoke(STR1(ARG0(__VA_ARGS__, )), __VA_ARGS__)

using namespace bench::tests::qdb;

template <typename Function, typename... Args>
static qdb_error_t named_invoke(const char * name, Function function, Args &&... args)
{
    qdb_error_t err = function(args...);
    if (QDB_FAILURE(err))
    {
        std::string error_str =
            fmt::format("{} returned [{:#08x}, {}]", utils::make_invocation_string(name, std::forward<Args>(args)...),
                        std::uint32_t(err), qdb_error(err));

        if ((err == qdb_e_system_local) || (err == qdb_e_system_remote))
        {
#ifdef _WIN32
            std::uint32_t lastError = utils::win32::get_last_error();
            error_str +=
                fmt::format(" && system returned [{}, {}]", lastError, utils::win32::get_error_string(lastError));
#else
            error_str += fmt::format(" && system returned [{}, {}]", errno, std::strerror(errno));
#endif
        }
        if (QDB_ERROR_SEVERITY(err) == qdb_e_severity_unrecoverable)
        {
            throw utils::fatal_error(error_str);
        }
        else
        {
            throw std::runtime_error(error_str);
        }
    }
    return err;
}

const std::map<std::string, qdb_encryption_t> quasardb_facade::_encrypt_map = {
    {"", qdb_crypt_none}, {"none", qdb_crypt_none}, {"aes_gcm_256", qdb_crypt_aes_gcm_256}};

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

void quasardb_facade::set_cluster_security(const std::string & cluster_public_file)
{
    std::ifstream f(cluster_public_file);
    auto key = std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    INVOKE(qdb_option_set_cluster_public_key, _handle, key.c_str());
}

void quasardb_facade::set_user_security(const std::string & user_credentials_file)
{
    std::ifstream f(user_credentials_file);
    auto user = std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    StaticJsonBuffer<200> jsonBuffer;

    JsonObject & root = jsonBuffer.parseObject(user.c_str());
    auto username = root["username"];
    auto password = root["secret_key"];
    INVOKE(qdb_option_set_user_credentials, _handle, username, password);
}

void quasardb_facade::set_encryption(const std::string & encryption_algorithm)
{
    const auto & it_encrypt_option = _encrypt_map.find(encryption_algorithm);
    if (it_encrypt_option != _encrypt_map.cend())
    {
        INVOKE(qdb_option_set_encryption, _handle, it_encrypt_option->second);
    }
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
    qdb_release(_handle, buffer);
}

std::string quasardb_facade::node_status(const std::string & node_uri) const
{
    const char * content;
    size_t content_size;
    INVOKE(qdb_node_status, _handle, node_uri.c_str(), &content, &content_size);
    std::string json(content, content_size);
    qdb_release(_handle, content);
    return json;
}

std::string quasardb_facade::node_topology(const std::string & node_uri) const
{
    const char * content;
    size_t content_size;
    INVOKE(qdb_node_topology, _handle, node_uri.c_str(), &content, &content_size);
    std::string json(content, content_size);
    qdb_release(_handle, content);
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

qdb_buffer quasardb_facade::deque_get_at(const std::string & alias, qdb_int_t index)
{
    const void * result;
    std::size_t result_size;
    INVOKE(qdb_deque_get_at, _handle, alias.c_str(), index, &result, &result_size);
    return qdb_buffer(_handle, result, result_size);
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

void quasardb_facade::attach_tag(const std::string & alias, const std::string & tag)
{
    INVOKE(qdb_attach_tag, _handle, alias.c_str(), tag.c_str());
}

void quasardb_facade::has_tag(const std::string & alias, const std::string & tag)
{
    INVOKE(qdb_has_tag, _handle, alias.c_str(), tag.c_str());
}

void quasardb_facade::detach_tag(const std::string & alias, const std::string & tag)
{
    INVOKE(qdb_detach_tag, _handle, alias.c_str(), tag.c_str());
}

void quasardb_facade::get_tagged(const std::string & tag)
{
    const char ** entries;
    size_t entry_count;
    INVOKE(qdb_get_tagged, _handle, tag.c_str(), &entries, &entry_count);
}

void quasardb_facade::get_tags(const std::string & alias)
{
    const char ** tags;
    size_t tag_count;
    INVOKE(qdb_get_tags, _handle, alias.c_str(), &tags, &tag_count);
}

void quasardb_facade::ts_create(const std::string & alias, const std::vector<qdb_ts_column_info_t> & columns)
{
    INVOKE(qdb_ts_create, _handle, alias.c_str(), columns.data(), columns.size());
}

void quasardb_facade::ts_col_blob_insert(const std::string & alias,
                                         const std::string & col_name,
                                         const qdb_timespec_t & ts,
                                         const std::string & content)
{
    qdb_ts_blob_point bp;

    bp.timestamp = ts;
    bp.content = content.data();
    bp.content_length = content.size();

    INVOKE(qdb_ts_blob_insert, _handle, alias.c_str(), col_name.c_str(), &bp, 1);
}

void quasardb_facade::ts_col_double_insert(const std::string & alias,
                                           const std::string & col_name,
                                           const qdb_timespec_t & ts,
                                           double content)
{
    qdb_ts_double_point dp;

    dp.timestamp = ts;
    dp.value = content;

    ts_col_double_inserts(alias, col_name, &dp, 1);
}

void quasardb_facade::ts_col_double_inserts(const std::string & alias,
                                            const std::string & col_name,
                                            const qdb_ts_double_point * points,
                                            size_t count)
{
    INVOKE(qdb_ts_double_insert, _handle, alias.c_str(), col_name.c_str(), points, count);
}

void quasardb_facade::ts_col_double_average(const std::string & alias,
                                            const std::string & col_name,
                                            const qdb_ts_range_t & range)
{
    qdb_ts_double_aggregation_t agg;

    agg.type = qdb_agg_arithmetic_mean;
    agg.range = range;

    INVOKE(qdb_ts_double_aggregate, _handle, alias.c_str(), col_name.c_str(), &agg, 1);
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
