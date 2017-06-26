#pragma once

#include <memory>

template <typename TO, typename FROM>
std::unique_ptr<TO> static_unique_pointer_cast(std::unique_ptr<FROM> && old)
{
    return std::unique_ptr<TO>{static_cast<TO *>(old.release())};
}