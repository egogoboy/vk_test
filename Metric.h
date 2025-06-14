#pragma once

#include "IMetric.h"

#include <string>
#include <atomic>

template <typename T>
class Metric : public IMetric{
public:
    Metric(const std::string& metric_name)
        : _metric_name{metric_name},
          _value{} {};

    void add_value(const T& value) {
        _value.store(value, std::memory_order_relaxed);
    }

    T get() const {
        return _value.load(std::memory_order_relaxed);
    }

    const std::string& name() const {
        return _metric_name;
    }

    std::pair<std::string, std::string> get_string_value() const override {
        return { _metric_name, std::to_string(_value.load()) };
    }

private:
    std::string _metric_name;
    std::atomic<T> _value;
};
