#include <bench/core/computations.hpp>
#include <algorithm>
#include <numeric>

namespace bench
{

static double sum_values(const bench::sample & sample)
{
    return static_cast<double>(std::accumulate(sample.values.begin(), sample.values.end(), std::int64_t()));
}

double compute_average_frequency(const test_instance & test)
{
    const sample & last_sample = test.result.at("test.iterations").samples.back();

    auto iterations = sum_values(last_sample);

    time_point stop_time = last_sample.time;
    duration elapsed = stop_time - test.start_time;
    auto elapsed_millis = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

    return 1000.0 * iterations / elapsed_millis;
}

double compute_average_throughput(const test_instance & test)
{
    return compute_average_frequency(test) * test.config.content_size * test.config.element_number;
}

std::int64_t compute_iteration_count(const test_instance & test)
{
    return static_cast<std::int64_t>(sum_values(test.result.at("test.iterations").samples.back()));
}

} // namespace bench
