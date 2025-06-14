#pragma once

#include "utils/Timestamp.h"
#include "MetricRegistry.h"

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <thread>

class MetricWriter {
public:
    MetricWriter(MetricRegistry& registry, const std::string& filename, int interval_ms);
    ~MetricWriter();

    void start();
    void stop();
    void on_console();
    void off_console();

private:
    MetricRegistry& _registry;
    std::string _filename;
    int _interval_ms;
    std::atomic<bool> _stop_flag;
    std::thread _main_thread;
    bool _console_out_flag;

    std::string _make_log_string();
};
