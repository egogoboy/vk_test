#include <metrics/MetricWriter.h>

metrics::MetricWriter::MetricWriter(MetricRegistry& reg, std::string file, uint64_t flush_ms,
             uint64_t bin_width_ms)
    : _reg(reg),
      _file(std::move(file)),
      _flush_ms(flush_ms),
      _bin_width(bin_width_ms) {}

metrics::MetricWriter::~MetricWriter() {
    stop();
}

void metrics::MetricWriter::start(bool to_console) {
    _to_console = to_console;
    _run = true;
    _th = std::thread(&MetricWriter::_loop, this);
}

void metrics::MetricWriter::stop() {
    _run = false;
    if (_th.joinable()) {
        _th.join();
        std::cout << "All metrics saved to " << _file << std::endl;
    }
}

void metrics::MetricWriter::_loop() {
    std::ofstream fout(_file, std::ios::app);
    if (!fout) {
        throw std::runtime_error("Cannot open " + _file);
    }
    while (_run) {
        auto start = std::chrono::steady_clock::now();

        _make_log(fout);

        auto spent = std::chrono::steady_clock::now() - start;
        auto sleep = std::chrono::milliseconds(_flush_ms) - spent;
        if (sleep.count() > 0) {
            std::this_thread::sleep_for(sleep);
        }
    }
    _make_log(fout);
}

void metrics::MetricWriter::_make_log(std::ofstream& fout) {
    std::vector<std::string> lines;
    _reg.make_lines(lines, _bin_width);
    for (auto& l : lines) {
        fout << l << '\n';
        if (_to_console) {
            std::cout << l << '\n';
        }
    }
    fout.flush();
}

void metrics::MetricWriter::to_console(bool f) {
    _to_console = f;
}
