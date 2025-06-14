#pragma once

#include "Metric.h"
#include "IMetric.h"

#include <vector>
#include <memory>
#include <mutex>

class MetricRegistry {
public:
    void add_metric(const std::shared_ptr<IMetric>& new_metric);
    std::vector<std::shared_ptr<IMetric>> get_all_metrics() const;

private:
    mutable std::mutex _mutex;
    std::vector<std::shared_ptr<IMetric>> _metrics;
};
