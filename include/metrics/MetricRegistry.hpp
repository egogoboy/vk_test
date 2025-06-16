#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <map>

#include <metrics/MetricSnapshot.hpp>
#include <metrics/MetricBuffered.hpp>
#include <utils/types.hpp>
#include <utils/utils.hpp>

namespace metrics {

class MetricRegistry {
public:
    template <typename T, template <typename> typename Impl = MetricSnapshot>
    std::shared_ptr<Impl<T>> create_metric(const std::string& name) {
        auto metric = std::make_shared<Impl<T>>(name);
        {
            std::lock_guard<std::mutex> lk(_mx);
            _metrics.push_back(metric);
        }
        return metric;
    }

    void make_lines(std::vector<std::string>& out, uint64_t bin_width_ms) {
        std::vector<std::shared_ptr<IMetric>> copy;
        {
            std::lock_guard<std::mutex> lk(_mx);
            copy = _metrics;
        }
        std::unordered_map<uint64_t, std::vector<std::pair<std::string, std::string>>> bins;
        for (auto& m : copy) m->collect(bins, bin_width_ms);

        for (auto& [ts_bin, kv] : bins) {
            std::ostringstream oss;
            oss << metric_time::ts_to_string(ts_bin);
            for (auto& [k, v] : kv) oss << " \"" << k << "\" " << v;
            out.push_back(std::move(oss).str());
        }
    }

private:
    std::mutex _mx;
    std::vector<std::shared_ptr<IMetric>> _metrics;
};

}
