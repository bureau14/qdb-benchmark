#include <utils/unique_alias.hpp>
#include <utils/watermark.hpp>

#include <fmt/format.h>
#include <atomic>
#include <chrono>
#ifdef _WIN32
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

static std::atomic<int> __counter(0);

// returns the ID of the current process
static std::uint32_t current_process_id() noexcept
{
#ifdef _WIN32
    return static_cast<std::uint32_t>(::GetCurrentProcessId());
#else
    return static_cast<std::uint32_t>(::getpid());
#endif
}

utils::unique_alias::unique_alias()
{
    using namespace std::chrono;
    auto pid = current_process_id();
    auto t = duration_cast<seconds>(steady_clock::now().time_since_epoch()).count();
    auto n = __counter++;

    _string = fmt::format("benchmarks-{}-{}-{}-AAAAAA", pid, t, n);
}

void utils::unique_alias::set_watermark(unsigned long iteration)
{
    utils::watermark::replace_end(_string, iteration);
}
