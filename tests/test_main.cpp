#include <metrics/MetricRegistry.hpp>
#include <metrics/MetricWriter.h>

#include <chrono>
#include <memory>
#include <thread>

int main() {
    std::cout << "Start standart preview test\n";
    metrics::MetricRegistry registry;

    auto cpu = registry.create_metric<double, metrics::MetricBuffered>("CPU");
    auto http = registry.create_metric<int>("HTTP requests RPS");

    metrics::MetricWriter writer(registry, "metrics.txt", 1000, 100);
    writer.to_console(false);
    writer.start(true);

    for (int i = 0; i < 100; ++i) {
        cpu->add(0.5 + std::rand() % 100 * 0.001);
        http->add(30 + std::rand() % 30);
        std::this_thread::sleep_for((std::chrono::milliseconds(100)));
    }

    writer.stop();
    std::cout << std::endl;

    return 0;
}
