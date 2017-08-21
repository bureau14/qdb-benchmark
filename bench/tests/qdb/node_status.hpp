#pragma once

#include <bench/core/probe.hpp>
#include <bench/tests/qdb/quasardb_facade.hpp>
#include <ArduinoJson.h>
#include <algorithm>
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
    node_status(std::string cluster_uri,
                std::string cluster_public_file = "",
                std::string user_credentials_file = "",
                std::string encryption_algorithm = "")
        : _cluster_uri(cluster_uri), _cluster_key(cluster_public_file), _user_config(user_credentials_file),
          _encryption_algorithm(encryption_algorithm)
    {
    }

    void setup() override
    {
        if (_cluster_key != "" && _user_config != "")
        {
            _quasardb.set_cluster_security(_cluster_key);
            _quasardb.set_user_security(_user_config);
            _quasardb.set_encryption(_encryption_algorithm);
        }

        for (auto node = first_node(); !has_node(node); node = get_successor(node))
        {
            add_node(node);
        }

        size_t columns = _node_uris.size();

        // clang-format off
        define_measurement("quasardb.node.physmem_used",    "Quasardb: Physical Memory",   unit::bytes, columns);
        define_measurement("quasardb.node.vm_used",         "Quasardb: Virtual Memory",    unit::bytes, columns);
        define_measurement("quasardb.node.evictions",       "Quasardb: Evictions",         unit::none,  columns);
        define_measurement("quasardb.node.persisted_size",  "Quasardb: Persisted Size",    unit::bytes, columns);
        define_measurement("quasardb.node.persisted_count", "Quasardb: Persisted Entries", unit::none,  columns);
        define_measurement("quasardb.node.resident_size",   "Quasardb: Resident Size",     unit::bytes, columns);
        define_measurement("quasardb.node.resident_count",  "Quasardb: Resident Entries",  unit::none,  columns);
        define_measurement("quasardb.node.successes",       "Quasardb: Successes",         unit::none,  columns);
        define_measurement("quasardb.node.failures",        "Quasardb: Failures",          unit::none,  columns);
        // clang-format on
    }

    void take_sample() override
    {
        for (std::size_t i = 0; i < _node_uris.size(); i++)
        {
            std::string json = _quasardb.node_status(_node_uris[i]);

            DynamicJsonBuffer jsonBuffer;
            JsonObject & node_status = jsonBuffer.parseObject(json.c_str());

            // clang-format off
            set_measured_value("quasardb.node.physmem_used",    i, node_status["memory"]["physmem"]["used"]);
            set_measured_value("quasardb.node.vm_used",         i, node_status["memory"]["vm"]["used"]);
            set_measured_value("quasardb.node.evictions",       i, node_status["overall"]["evictions"]);
            set_measured_value("quasardb.node.persisted_size",  i, node_status["entries"]["persisted"]["size"]);
            set_measured_value("quasardb.node.persisted_count", i, node_status["entries"]["persisted"]["count"]);
            set_measured_value("quasardb.node.resident_size",   i, node_status["entries"]["resident"]["size"]);
            set_measured_value("quasardb.node.resident_count",  i, node_status["entries"]["resident"]["count"]);
            set_measured_value("quasardb.node.successes",       i, node_status["overall"]["successes"]);
            set_measured_value("quasardb.node.failures",        i, node_status["overall"]["failures"]);
            // clang-format on
        }
    }

    void cleanup() override
    {
    }

private:
    void add_node(const std::string & node_uri)
    {
        _node_uris.push_back(node_uri);
    }

    bool has_node(const std::string & node_uri) const
    {
        return std::find(_node_uris.begin(), _node_uris.end(), node_uri) != _node_uris.end();
    }

    std::string first_node() const
    {
        // TODO: this assume that only one node is present in the URI, we shoudld extract the node's
        // endpoint from the cluster URI
        return _cluster_uri;
    }

    std::string get_successor(const std::string & node_uri) const
    {
        DynamicJsonBuffer jsonBuffer;
        std::string json = _quasardb.node_topology(node_uri);
        JsonObject & topology = jsonBuffer.parseObject(json.c_str());
        std::string endpoint = topology["successor"]["endpoint"];
        return "qdb://" + endpoint;
    }

    std::string _cluster_uri;
    std::string _cluster_key;
    std::string _user_config;
    std::string _encryption_algorithm;
    quasardb_facade _quasardb;
    std::vector<std::string> _node_uris;
};

} // namespace qdb
} // namespace tests
} // namespace bench
