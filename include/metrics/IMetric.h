#pragma once

#include <string>

class IMetric {
public:
    virtual ~IMetric() = default;
    virtual std::pair<std::string, std::string> get_string_value() const = 0;
};
