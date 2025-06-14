#pragma once

#include "MetricRegistry.h"

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <thread>

class MetricWriter {
public:
    MetricWriter(MetricRegistry& registry, const std::string& filename, int interval_ms)
        : _registry{registry}, _filename{filename}, _interval_ms{interval_ms}, _stop_flag{false} {
        _main_thread = std::thread();
    }

    void start() {
        _main_thread = std::thread([this]() {
            std::ofstream fout(_filename);
            if (!fout.is_open()) {
                std::cerr << "Unable to open " << _filename << " file." << std::endl;
                return 0;
            }

            while (!_stop_flag.load()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(_interval_ms));
                auto metrics = _registry.get_all_metrics();
                fout << _get_current_timestamp();
                for (const auto& metric : metrics) {
                    auto [name, value] = metric->get_string_value();
                    fout << " \"" << name << "\" " << value;
                }
                fout << std::endl;
            }
        });
    }

    void stop() {
        _stop_flag.store(true);
        if (_main_thread.joinable()) {
            _main_thread.join();
        }
    }

    ~MetricWriter() {
        stop();
    }

private:
    MetricRegistry& _registry;
    std::string _filename;
    int _interval_ms;
    std::atomic<bool> _stop_flag;
    std::thread _main_thread;

    std::string _get_current_timestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_t_now), "%F %T")
            << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return oss.str();
    }
};
