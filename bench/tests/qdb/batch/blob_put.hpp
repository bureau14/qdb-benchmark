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
    }

    void setup() override
    {
        qdb_test_template::setup();
    }

    void run_iteration(std::uint32_t iteration)
    {
        auto start = iteration * _number_of_elements;

        auto contents = std::vector<std::string>(_number_of_elements);
        auto aliases = std::vector<std::string>(_number_of_elements);
        auto operations = std::vector<qdb_operation_t>(_number_of_elements);
        for (std::size_t i = 0; i < _number_of_elements; ++i)
        {
            contents[i] = content(start + i);
            aliases[i] = alias(start + i);
            operations[i].type = qdb_op_blob_put;
            operations[i].alias = aliases[i].c_str();
            operations[i].blob_put.content = contents[i].c_str();
            operations[i].blob_put.content_size = contents[i].size();
        }
        auto success_count = _qdb.run_batch(operations);
        if (success_count != _number_of_elements)
        {
            for (const auto & op : operations)
            {
                if ((op.error != qdb_e_ok) && (op.error != qdb_e_ok_created))
                {
                    std::cout << fmt::format("\n{} - {} - {}", qdb_error(op.error), std::string(op.alias),
                                             std::string(static_cast<const char *>(op.blob_put.content)));
                }
            }
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
};
} // namespace batch
} // namespace qdb
} // namespace tests
} // namespace bench
