#pragma once

#include <qdb/client.h>
#include <qdb/ts.h>
#include <bench/tests/qdb/qdb_test_template.hpp>
#include <cppformat/format.h>
#include <utils/flex_barrier.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace bench
{
namespace tests
{
namespace qdb
{
namespace ts
{
class multicolumns_double_insert : public qdb_test_template<multicolumns_double_insert>
{

public:
    explicit multicolumns_double_insert(bench::test_config config)
        : qdb_test_template(config), _number_of_columns(config.content_size),
          _finish_barrier(config.content_size, [this] { return _number_of_columns; })
    {
    }

    void setup() override
    {
        qdb_test_template::setup();

        std::vector<std::string> columns_names = generate_columns_names();
        std::vector<qdb_ts_column_info_t> columns_info = generate_columns_info(columns_names);
        _qdb.ts_create(alias(0), columns_info);
        generate_threads(columns_names);
    }

    void run_iteration(std::uint32_t iteration)
    {
        _columns_done = 0;
        _fire_threads = true;
        _cv_fire_threads.notify_all();
        {
            std::unique_lock<std::mutex> local_lock{_lock};
            _cv_wake_main.wait(local_lock, [this] { return _wake_main == true; });
            _wake_main = false;
        }
    }

    void cleanup() override
    {
        _shutdown = true;
        _fire_threads = true;
        _cv_fire_threads.notify_all();
        for (auto & thread : _threads)
        {
            thread.join();
        }
        _qdb.remove(alias(0));
        qdb_test_template::cleanup();
    }

    static std::string name()
    {
        return "qdb_ts_multicolumns_double_insert";
    }

    static std::string description()
    {
        return "Each thread inserts a double point in the same time series column";
    }

    static bool size_dependent()
    {
        return true;
    }

private:
    const size_t _number_of_columns;
    std::vector<std::thread> _threads;
    std::condition_variable _cv_wake_main;
    std::condition_variable _cv_fire_threads;
    std::atomic_bool _wake_main;
    std::atomic_bool _fire_threads;
    std::atomic<size_t> _columns_done;
    std::mutex _lock;
    bool _shutdown = false;
    utils::flex_barrier _finish_barrier;

    void thread_insertion(std::string alias, std::string col_name, qdb_time_t timestamp)
    {
        auto caller = [alias, col_name, this](qdb_time_t timestamp) {
            thread_local qdb_handle_t handle = []() -> qdb_handle_t {
                qdb_handle_t h = qdb_open_tcp();
                qdb_connect(h, "qdb://127.0.0.1:2836");
                return h;
            }();
            static qdb_time_t second_timestamp = timestamp;
            qdb_ts_double_point double_point{qdb_timespec_t{second_timestamp, timestamp}, generate_value()};
            qdb_ts_double_insert(handle, alias.c_str(), col_name.c_str(), &double_point, 1);
        };
        while (1)
        {
            {
                std::unique_lock<std::mutex> local_lock{_lock};
                _cv_fire_threads.wait(local_lock, [this] { return _fire_threads == true; });
            }
            if (_shutdown) return;

            caller(timestamp);
            timestamp += 33;
            _finish_barrier.arrive_and_wait();
            if (_fire_threads == true)
            {
                _fire_threads = false;
            }
            ++_columns_done;
            if (_columns_done == _number_of_columns)
            {
                _wake_main = true;
                _cv_wake_main.notify_one();
            }
        }
    }

    qdb_time_t generate_timestamp()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<qdb_time_t> dist_time(0, 1490206139);

        return dist_time(gen);
    }

    double generate_value()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<double> dist_double(0, 10);

        return dist_double(gen);
    }

    std::vector<std::string> generate_columns_names()
    {
        std::vector<std::string> columns_names(_number_of_columns);
        auto i = 0;
        std::generate(columns_names.begin(), columns_names.end(), [&i]() {
            const auto column_base_name = std::string("column-");
            return std::string(column_base_name + std::to_string(i++));
        });
        return columns_names;
    }

    std::vector<qdb_ts_column_info_t> generate_columns_info(const std::vector<std::string> & columns_names)
    {
        std::vector<qdb_ts_column_info_t> columns_info(_number_of_columns);
        auto i = 0;
        std::generate(columns_info.begin(), columns_info.end(), [&i, this, columns_names]() {
            return qdb_ts_column_info_t{columns_names[i++].c_str(), qdb_ts_column_double};
        });
        return columns_info;
    }

    void generate_threads(const std::vector<std::string> & columns_names)
    {
        _threads.reserve(_number_of_columns);
        qdb_time_t timestamp = generate_timestamp();
        for (size_t i = 0; i < _number_of_columns; ++i)
        {
            _threads.emplace_back(
                std::bind(&multicolumns_double_insert::thread_insertion, this, alias(0), columns_names[i], timestamp));
        }
    }
};
} // namespace ts
} // namespace qdb
} // namespace tests
} // namespace bench
