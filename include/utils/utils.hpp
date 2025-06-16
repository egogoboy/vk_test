#pragma once

#include <chrono>
#include <sstream>
#include <iomanip>
#include <string>

namespace metric_time {

    inline uint64_t now_ms() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
            .count();
    }

    inline std::string ts_to_string(uint64_t ms) {
        using namespace std::chrono;
        auto tp   = system_clock::time_point(milliseconds(ms));
        auto time = system_clock::to_time_t(tp);
        auto ms_part = duration_cast<milliseconds>(tp.time_since_epoch()) % 1000;

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time), "%F %T") << '.'
            << std::setfill('0') << std::setw(3) << ms_part.count();
        return oss.str();
    }

}
