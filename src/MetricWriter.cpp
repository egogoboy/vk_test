#include <metrics/MetricWriter.h>
#include <sstream>

MetricWriter::MetricWriter(MetricRegistry& registry, const std::string& filename, int interval_ms)
    : _registry{registry}, 
      _filename{filename}, 
      _interval_ms{interval_ms}, 
      _stop_flag{false},
      _console_out_flag{false} {}

MetricWriter::~MetricWriter() {
    stop();
}

void MetricWriter::start() {
    _main_thread = std::thread([this]() {
        std::ofstream fout(_filename);
        if (!fout.is_open()) {
            std::cerr << "Unable to open " << _filename << " file." << std::endl;
            return 0;
        }
        
        std::string log_string;
        while (!_stop_flag.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(_interval_ms));
            log_string = _make_log_string(); 

            fout << log_string;
            if (_console_out_flag) {
                std::cout << log_string;
            }
        }
    });
}

void MetricWriter::stop() {
    _stop_flag.store(true);
    if (_main_thread.joinable()) {
        _main_thread.join();
        std::cout << "All metrics saved to " << _filename << "\n";
    }
}

std::string MetricWriter::_make_log_string() {
    std::ostringstream oss;
    auto metrics = _registry.get_all_metrics();

    oss << timestamp::get_current_timestamp();
    for (const auto& metric : metrics) {
        auto [name, value] = metric->get_string_value();
        oss << " \"" << name << "\" " << value;
    }
    oss << "\n";

    return oss.str();
}

void MetricWriter::on_console() {
    _console_out_flag = true;
}

void MetricWriter::off_console() {
    _console_out_flag = false;
}
