#pragma once

#include <qdb/batch.h>
#include <bench/tests/qdb/qdb_test_template.hpp>
#include <iostream>

namespace bench
{
namespace tests
{
namespace qdb
{
namespace batch
{
class blob_put : public qdb_test_template<blob_put>
{
public:
    explicit blob_put(bench::test_config config) : qdb_test_template(config), _number_of_elements(config.element_number)
    {
        _contents = std::vector<std::string>(_number_of_elements);
        _aliases = std::vector<std::string>(_number_of_elements);
        _operations = std::vector<qdb_operation_t>(_number_of_elements);
    }

    void setup() override
    {
        qdb_test_template::setup();
    }

    void run_iteration(std::uint32_t iteration)
    {
        auto start = iteration * _number_of_elements;

        for (std::size_t i = 0; i < _number_of_elements; ++i)
        {
            _contents[i] = content(start + i);
            _aliases[i] = alias(start + i);
            _operations[i].type = qdb_op_blob_put;
            _operations[i].alias = _aliases[i].c_str();
            _operations[i].blob_put.content = _contents[i].c_str();
            _operations[i].blob_put.content_size = _contents[i].size();
        }
        auto success_count = _qdb.run_batch(_operations);
        if (success_count != _number_of_elements)
        {
            throw std::runtime_error("error");
        }
    }

    void cleanup() override
    {
        cleanup_each([=](std::uint32_t iteration) {
            auto start = iteration * _number_of_elements;
            for (std::size_t i = 0; i < _number_of_elements; ++i)
            {
                _qdb.remove(alias(start + i));
            }
        });
        qdb_test_template::cleanup();
    }

    static std::string name()
    {
        return "qdb_batch_blob_put";
    }

    static std::string description()
    {
        return "Each thread repeats qdb_run_batch() with new operations";
    }

    static bool size_dependent()
    {
        return true;
    }

private:
    std::size_t _number_of_elements;
    std::vector<std::string> _contents;
    std::vector<std::string> _aliases;
    std::vector<qdb_operation_t> _operations;
};
} // namespace batch
} // namespace qdb
} // namespace tests
} // namespace bench
