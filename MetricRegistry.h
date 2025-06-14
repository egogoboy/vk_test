#pragma once

#include "Metric.h"
#include "IMetric.h"

#include <vector>
#include <memory>
#include <mutex>

class MetricRegistry {
public:
    void add_metric(const std::shared_ptr<IMetric>& new_metric) {
        std::lock_guard<std::mutex> lock(_mutex);
        _metrics.push_back(new_metric);
    }

    std::vector<std::shared_ptr<IMetric>> get_all_metrics() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return _metrics;
    }

private:
    mutable std::mutex _mutex;
    std::vector<std::shared_ptr<IMetric>> _metrics;
};
