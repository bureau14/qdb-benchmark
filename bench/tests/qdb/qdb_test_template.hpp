#pragma once

#include <bench/tests/common/test_template.hpp>
#include <bench/tests/qdb/node_status.hpp>
#include <bench/tests/qdb/quasardb_facade.hpp>
#include <utils/unique_alias_provider.hpp>
#include <utils/unique_content_provider.hpp>

namespace bench
{
namespace tests
{
namespace qdb
{
template <typename Derived>
class qdb_test_template : public test_template<Derived>,
                          protected utils::unique_alias_provider,
                          protected utils::unique_content_provider
{
public:
    qdb_test_template(test_config config)
        : test_template<Derived>(config), unique_content_provider(config.content_size),
          _cluster_uri(config.cluster_uri), _cluster_key(config.cluster_public_file),
          _user_config(config.user_credentials_file), _encryption_algorithm(config.encryption_algorithm)
    {
    }

    void setup() override
    {
        if (_cluster_key != "" && _user_config != "")
        {
            _qdb.set_cluster_security(_cluster_key);
            _qdb.set_user_security(_user_config);
            _qdb.set_encryption(_encryption_algorithm);
        }
        _qdb.connect(_cluster_uri);
    }

    void cleanup() override
    {
        // _qdb.trim_all(); <- times out
        _qdb.close();
    }

    static probe_collection create_probes(test_config cfg)
    {
        probe_collection probes;
        probes.emplace_back(new node_status(cfg.cluster_uri, cfg.cluster_public_file, cfg.user_credentials_file,
                                            cfg.encryption_algorithm));
        return probes;
    }

protected:
    quasardb_facade _qdb;

private:
    std::string _cluster_uri;
    std::string _cluster_key;
    std::string _user_config;
    std::string _encryption_algorithm;
};

} // namespace qdb
} // namespace tests
} // namespace bench
