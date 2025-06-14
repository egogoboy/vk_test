#include <metrics/Metric.h>
#include <metrics/MetricRegistry.h>
#include <metrics/MetricWriter.h>

#include <chrono>
#include <memory>
#include <thread>

int main() {
    MetricRegistry registry;

    auto cpu = std::make_shared<Metric<double>>("CPU");
    auto http = std::make_shared<Metric<int>>("HTTP requets RPS");

    registry.add_metric(cpu);
    registry.add_metric(http);

    MetricWriter writer(registry, "metrics.txt", 100);
    writer.on_console();
    writer.start();

    for (int i = 0; i < 100; ++i) {
        cpu->add_value(0.5 + std::rand() % 100 * 0.001);
        http->add_value(30 + std::rand() % 30);
        std::this_thread::sleep_for((std::chrono::milliseconds(100)));
    }
    
    return 0;
}
