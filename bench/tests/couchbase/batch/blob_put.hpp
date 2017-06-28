#pragma once

#include <bench/tests/couchbase/couchbase_test_template.hpp>

namespace bench
{
namespace tests
{
namespace couchbase
{
namespace batch
{
class blob_put : public couchbase_test_template<blob_put>
{
public:
    explicit blob_put(bench::test_config config)
        : couchbase_test_template(config), _number_of_elements(config.element_number)
    {
    }

    void run_iteration(std::uint32_t iteration)
    {
        auto start = iteration * _number_of_elements;

        auto contents = std::vector<std::string>(_number_of_elements);
        auto aliases = std::vector<std::string>(_number_of_elements);
        auto cmds = std::vector<lcb_CMDSTORE>(_number_of_elements);
        for (std::size_t i = 0; i < _number_of_elements; ++i)
        {
            aliases[i] = alias(start + i);
            contents[i] = content(start + i);
            cmds[i] = {0};
            LCB_CMD_SET_KEY(&cmds[i], aliases[i].c_str(), aliases[i].length());
            LCB_CMD_SET_VALUE(&cmds[i], contents[i].c_str(), contents[i].length());
            cmds[i].operation = LCB_ADD;
        }
        _couchbase.run_batch(cmds);
    }

    void cleanup() override
    {
        cleanup_each([=](std::uint32_t iteration) { _couchbase.remove(alias(iteration)); });
    }

    static std::string name()
    {
        return "couchbase_batch_blob_put";
    }

    static std::string description()
    {
        return "Each thread repeats couchbase.insert with new ids";
    }

    static bool size_dependent()
    {
        return true;
    }

private:
    std::size_t _number_of_elements;
};
} // namespace blob
} // namespace couchbase
} // namespace tests
} // namespace bench
