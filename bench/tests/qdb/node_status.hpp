#pragma once

#include <bench/core/probe.hpp>
#include <utils/qdb_wrapper.hpp>
#include <rapidjson/document.h>

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

namespace bench
{
namespace tests
{
namespace qdb
{

class node_status : public probe
{
public:
    node_status(::std::string cluster_uri) : _cluster_uri(cluster_uri)
    {
    }

    void setup() override
    {
        for (auto node = first_node(); !has_node(node); node = get_successor(node))
        {
            add_node(node);
        }
    }

    void take_sample(time_point now, result_type & result) override
    {
        ::std::vector<::std::int64_t> physmem_used, vm_used, evictions;
        ::std::vector<::std::int64_t> persisted_size, persisted_count;
        ::std::vector<::std::int64_t> resident_size, resident_count;
        ::std::vector<::std::int64_t> successes, failures;

        for (auto node : _node_uris)
        {
            auto node_status = get_node_status(node);
            physmem_used.push_back(node_status["memory"]["physmem"]["used"].GetInt64());
            vm_used.push_back(node_status["memory"]["vm"]["used"].GetInt64());
            evictions.push_back(node_status["overall"]["evictions"].GetInt64());
            persisted_size.push_back(node_status["entries"]["persisted"]["size"].GetInt64());
            persisted_count.push_back(node_status["entries"]["persisted"]["count"].GetInt64());
            resident_size.push_back(node_status["entries"]["resident"]["size"].GetInt64());
            resident_count.push_back(node_status["entries"]["resident"]["count"].GetInt64());
            successes.push_back(node_status["overall"]["successes"].GetInt64());
            failures.push_back(node_status["overall"]["failures"].GetInt64());
        }

        result["physmem_used"].push_back({now, physmem_used});
        result["vm_used"].push_back({now, vm_used});
        result["evictions"].push_back({now, evictions});
        result["persisted_size"].push_back({now, persisted_size});
        result["persisted_count"].push_back({now, persisted_count});
        result["resident_size"].push_back({now, resident_size});
        result["resident_count"].push_back({now, resident_count});
        result["successes"].push_back({now, successes});
        result["failures"].push_back({now, failures});
    }

    void cleanup() override
    {
    }

private:
    rapidjson::Document get_node_status(const ::std::string & node_uri) const
    {
        rapidjson::Document doc;
        utils::qdb_buffer json = _qdb.node_status(node_uri);
        doc.Parse(json.data());
        return doc;
    }

    rapidjson::Document get_node_topology(const ::std::string & node_uri) const
    {
        rapidjson::Document doc;
        utils::qdb_buffer json = _qdb.node_topology(node_uri);
        doc.Parse(json.data());
        return doc;
    }

    void add_node(const ::std::string & node_uri)
    {
        _node_uris.push_back(node_uri);
    }

    bool has_node(const ::std::string & node_uri) const
    {
        return ::std::find(_node_uris.begin(), _node_uris.end(), node_uri) != _node_uris.end();
    }

    ::std::string first_node() const
    {
        // TODO: this assume that only one node is present in the URI, we shoudld extract the node's
        // endpoint from the cluster URI
        return _cluster_uri;
    }

    ::std::string get_successor(const ::std::string & node_uri) const
    {
        auto topology = get_node_topology(node_uri);
        ::std::string endpoint = topology["successor"]["endpoint"].GetString();
        return "qdb://" + endpoint;
    }

    ::std::string _cluster_uri;
    utils::qdb_wrapper _qdb;
    ::std::vector<::std::string> _node_uris;
};

} // namespace qdb
} // namespace tests
} // namespace bench
