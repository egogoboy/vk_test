#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <vector>

class IMetric {
public:
    virtual ~IMetric() = default;
    virtual const std::string& name() const = 0;
    virtual void collect(
            std::map<uint64_t, std::vector<std::pair<std::string, std::string>>>& bins, 
            uint64_t bin_width_ms) = 0;
};
