#pragma once

#include <string>
#include <cstdint>

namespace metrics {

    enum class RecordMode { Snapshot, Buffered };

    struct Sample {
        uint64_t    ts_ms;
        std::string value;
    };

}
