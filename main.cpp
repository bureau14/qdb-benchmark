#include <bench/app/program.hpp>
#include <bench/log/console_logger.hpp>
#include <bench/tests/dummy/dummy_tests.hpp>
#include <bench/tests/std/std_tests.hpp>

#if BENCHMARK_CASSANDRA
#include <bench/tests/cassandra/cassandra_tests.hpp>
#endif
#if BENCHMARK_MONGODB
#include <bench/tests/mongodb/mongodb_tests.hpp>
#endif
#if BENCHMARK_INFLUXDB
#include <bench/tests/influxdb/influxdb_tests.hpp>
#endif
#if BENCHMARK_QUASARDB
#include <bench/tests/qdb/quasardb_tests.hpp>
#endif

#include <cstdlib>
#include <iostream>
#include <iterator>

static bench::log::logger & get_logger()
{
    const char * teamcity_version = std::getenv("TEAMCITY_VERSION");
    if (teamcity_version && teamcity_version[0])
    {
        return bench::log::get_teamcity_logger();
    }
    return bench::log::get_console_logger();
}

static bench::test_class_collection get_tests()
{
    bench::test_class_collection result;

#if DEBUG
    bench::tests::dummy::get_tests(std::back_inserter(result));
#endif
#if BENCHMARK_CASSANDRA
    bench::tests::cassandra::get_tests(std::back_inserter(result));
#endif
#if BENCHMARK_MONGODB
    bench::tests::mongodb::get_tests(std::back_inserter(result));
#endif
#if BENCHMARK_INFLUXDB
    bench::tests::influxdb::get_tests(std::back_inserter(result));
#endif
#if BENCHMARK_QUASARDB
    bench::tests::qdb::get_tests(std::back_inserter(result));
#endif
    bench::tests::std_::get_tests(std::back_inserter(result));

    return result;
}

int main(int argc, const char * argv[])
{
    bench::log::logger & logger = get_logger();
    bench::app::program program(logger, get_tests());

    try
    {
        program.run(argc, argv);
        return EXIT_SUCCESS;
    }
    catch (std::exception & e)
    {
        logger.fatal_error(e.what());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        logger.fatal_error("Unknown exception!");
        return EXIT_FAILURE;
    }
}
