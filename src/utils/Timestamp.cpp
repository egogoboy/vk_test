#include "utils/Timestamp.h"
#include <chrono>
#include <iomanip>

namespace timestamp {
    std::string get_current_timestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_t_now), "%F %T")
            << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return oss.str();
    }
}
