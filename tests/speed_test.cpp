#include <cstdlib>
#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>

#include <metrics/MetricSnapshot.hpp>
#include <metrics/MetricRegistry.hpp>
#include <metrics/MetricWriter.h>

void some_work() {
    std::vector<std::string> strings(1000);
    for (int i = 0; i < 1000; ++i) {
        strings[i] = std::to_string(std::rand());
    }
    for (int i = 0; i < 1000; ++i) {
        std::reverse(strings[i].begin(), strings[i].end());
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::reverse(strings.begin(), strings.end());
}

void some_work(std::shared_ptr<metrics::MetricSnapshot<int>> metric) {
    std::vector<std::string> strings(1000);
    for (int i = 0; i < 1000; ++i) {
        strings[i] = std::to_string(std::rand());
        metric->add(strings[i].length());
    }
    for (int i = 0; i < 1000; ++i) {
        std::reverse(strings[i].begin(), strings[i].end());
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        metric->add(strings[i].length());
    }
    std::reverse(strings.begin(), strings.end());
}

void some_work(std::shared_ptr<metrics::MetricBuffered<int>> metric) {
    std::vector<std::string> strings(1000);
    for (int i = 0; i < 1000; ++i) {
        strings[i] = std::to_string(std::rand());
        metric->add(strings[i].length());
    }
    for (int i = 0; i < 1000; ++i) {
        std::reverse(strings[i].begin(), strings[i].end());
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        metric->add(strings[i].length());
    }
    std::reverse(strings.begin(), strings.end());
}

using namespace std::chrono;

int main() {
    std::cout << "Speed test started\n";
    auto start_time = steady_clock::now();
    std::cout << "Start work without metrics\n";
    some_work();
    std::cout << "End work without metrics\n";
    auto end_time = steady_clock::now();
    milliseconds clean_run_time = duration_cast<milliseconds>(end_time - start_time);


    metrics::MetricRegistry registry;
    auto snapshot_metric = registry.create_metric<int>("string_length_now");
    auto buffered_metric = registry.create_metric<int, metrics::MetricBuffered>("string_length_now");

    metrics::MetricWriter writer(registry, "speed_metrics.txt", 10, 10);

    start_time = steady_clock::now();
    writer.start();
    std::cout << "Start work with snapshot metrics\n";
    some_work(snapshot_metric);
    std::cout << "End work with snapshot metrics\n";
    end_time = steady_clock::now();
    milliseconds snapshot_run_time = duration_cast<milliseconds>(end_time - start_time);


    char choice = 't';
    do {
        std::cout << "Turn on console? y/n\n";
        std::cin >> choice;
    } while (std::tolower(choice) != 'y' && std::tolower(choice) != 'n');

    if (choice == 'y') {
        writer.to_console(true);
    }

    start_time = steady_clock::now();
    std::cout << "Start work with buffered metrics\n";
    some_work(buffered_metric);
    std::cout << "End work with buffered metrics\n";
    end_time = steady_clock::now();
    milliseconds buffered_run_time = duration_cast<milliseconds>(end_time - start_time);

    writer.stop();
    std::cout << std::endl;

    std::cout << "Runtime without metrics: " << clean_run_time.count() << std::endl;
    std::cout << "Runtime with snapshot metrics: " << snapshot_run_time.count() << std::endl;
    std::cout << "Runtime with bufered metrics and console: " << buffered_run_time.count() << std::endl;
    return 0;
}
