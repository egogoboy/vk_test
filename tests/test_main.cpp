#include <utils/types.hpp>
#include <metrics/MetricRegistry.hpp>
#include <metrics/MetricWriter.h>

#include <chrono>
#include <memory>
#include <thread>

int main() {
    metrics::MetricRegistry registry;

    auto cpu = registry.create_metric<double, metrics::MetricBuffered>("CPU");
    auto http = registry.create_metric<int>("HTTP requests RPS");

    metrics::MetricWriter writer(registry, "metrics.txt", 1000, 1);
    writer.to_console(false);
    writer.start(true);

    for (int i = 0; i < 100; ++i) {
        cpu->add(0.5 + std::rand() % 100 * 0.001);
        http->add(30 + std::rand() % 30);
        std::cout << i << std::endl;
        std::this_thread::sleep_for((std::chrono::milliseconds(50)));
    }
    
    return 0;
}
