#pragma once

#include <mutex>
#include <deque>
#include <unordered_map>
#include <vector>
#include <string>

#include <metrics/IMetric.hpp>
#include <utils/utils.hpp>
#include <utils/types.hpp>

namespace metrics {
    template <typename T>
    class MetricBuffered final : public IMetric {
    public:
        explicit MetricBuffered(std::string n) : _name(std::move(n)) {}

        void add(const T& v) {
            std::lock_guard<std::mutex> lk(_mx);
            _q.push_back({ metric_time::now_ms(), std::to_string(v) });
        }

        const std::string& name() const override {
            return _name;
        }

        void collect(std::map<uint64_t,
                                       std::vector<std::pair<std::string, std::string>>>& bins,
                     uint64_t bin_width_ms) override {
            std::deque<metrics::Sample> local;
            {
                std::lock_guard<std::mutex> lk(_mx);
                local.swap(_q);
            }
            for (auto& s : local) {
                auto bin = s.ts_ms / bin_width_ms * bin_width_ms;
                bins[bin].push_back({ _name, std::move(s.value) });
            }
        }

    private:
        std::string _name;
        std::mutex  _mx;
        std::deque<metrics::Sample> _q;
    };
}
