#pragma once

#include <mutex>
#include <atomic>

#include <metrics/IMetric.hpp>
#include <utils/utils.hpp>

namespace metrics {
    template <typename T>
    class MetricSnapshot final : public IMetric {
    public:
        explicit MetricSnapshot(std::string n) : _name(std::move(n)) {}

        void add(const T& v) {
            _val.store(v, std::memory_order_relaxed);
        }

        const std::string& name() const override {
            return _name;
        }

        void collect(std::map<uint64_t,
                                       std::vector<std::pair<std::string, std::string>>>& bins,
                     uint64_t bin_width_ms) override {
            auto ts  = metric_time::now_ms();
            auto bin = ts / bin_width_ms * bin_width_ms;
            bins[bin].push_back({ _name, std::to_string(_val.load(std::memory_order_relaxed)) });
        }

    private:
        std::string  _name;
        std::atomic<T> _val{0};
    };
}
