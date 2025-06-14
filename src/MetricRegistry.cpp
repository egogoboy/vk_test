#include <metrics/MetricRegistry.h>

void MetricRegistry::add_metric(const std::shared_ptr<IMetric>& new_metric) {
    std::lock_guard<std::mutex> lock(_mutex);
    _metrics.push_back(new_metric);
}

std::vector<std::shared_ptr<IMetric>> MetricRegistry::get_all_metrics() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _metrics;
}
