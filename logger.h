#pragma once
// Jakub Staroń, 2016

#include "io.h"

namespace lib {
namespace logging {

enum class log_level {
  debug = 15,
  info = 10,
  error = 5
};

class Logger {
public:
  friend Logger& get_logger(const std::string& name);

  Logger(Logger&& other):
      level_(other.level_),
      streams_(std::move(other.streams_)) { }

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  template <typename... Args>
  void log(log_level level, const char* format, const Args&... args) {
    if (level > level_)
      return;

    for (const auto& pair: streams_) {
      if (level > pair.second)
        continue;

      print(*pair.first, format, args...);
    }
  }

  template <typename... Args>
  void debug(const char* format, const Args&... args) {
    log(log_level::debug, format, args...);
  }

  template <typename... Args>
  void info(const char* format, const Args&... args) {
    log(log_level::info, format, args...);
  }

  template <typename... Args>
  void error(const char* format, const Args&... args) {
    log(log_level::error, format, args...);
  }

  void add_stream(std::ostream* stream, log_level level) {
    streams_.emplace_back(stream, level);
  }

  void set_log_level(log_level level) {
    level_ = level;
  }

protected:
  Logger() = default;

private:
  log_level level_ = log_level::debug;
  std::vector<std::pair<std::ostream*, log_level>> streams_;
};

Logger& get_logger(const std::string& name) {
  static std::map<std::string, Logger> loggers;
  auto it = loggers.find(name);

  if (it == loggers.end()) {
    it = loggers.emplace(name, Logger()).first;
  }
  return it->second;
}

} // namespace logging
} // namespace lib
