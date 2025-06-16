#pragma once

#include <condition_variable>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <thread>

#include <metrics/MetricRegistry.hpp>

namespace metrics {

class MetricWriter {
public:
    MetricWriter(MetricRegistry& reg, std::string file, uint64_t flush_ms = 1000,
                 uint64_t bin_width_ms = 1);
    ~MetricWriter();

    void start(bool to_console = false);
    void stop();

    void to_console(bool f = false);

private:
    void _loop();
    void _make_log(std::ofstream& fout);

    MetricRegistry& _reg;
    std::string _file;
    uint64_t _flush_ms;
    uint64_t _bin_width;
    bool _to_console{false};
    std::atomic<bool> _run{false};
    std::thread _th;
};

}
