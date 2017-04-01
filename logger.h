#pragma once
// Jakub Staroń, 2016-2017

#include "io.h"
#include "text_algorithms/string.h"

namespace pcl {
namespace logging {

enum class log_level {
  debug = 15,
  info = 10,
  error = 5,
  none = 0
};

const char* log_level_name(log_level level) {
  switch (level) {
    case log_level::debug:
      return "debug";
    case log_level::info:
      return "info";
    case log_level::error:
      return "error";
    case log_level::none:
      return "none";
  }
}

namespace detail {

std::string date_time_string() {
  std::ostringstream stream;
  std::time_t t = std::time(nullptr);
  std::tm tm = *std::localtime(&t);
  stream << std::put_time(&tm, "%d/%m/%y %T");
  return stream.str();
}

} // namespace detail

class LogMaster {
public:
  friend LogMaster& log_master();

  /**
   * Subscribes given topic with given log level using given message format
   * and given stream.
   *
   * Message format is a string consisting of given letters:
   * * T - topic
   * * L - log level
   * * D - date and time
   *
   * Examples:
   * format "TLD" will produce log entry "[topic] [log_level] [date_time] Message"
   * format "" will produce "Message"
   */
  void subscribe(std::string topic, log_level level, std::string format, std::ostream* stream) {
    subscriptions_.push_back({std::move(topic), level, std::move(format), stream});
  }

  /**
   * Pushes given log message with givel log level on given topic.
   */
  void log(const std::string& topic, log_level level, const std::string& message) {
    for (const auto& subscription: subscriptions_) {
      // TODO Fix topic logic
      // Now "ala" is common topic to both "alamakota" and "ala.ma.kota".
      // Only "ala.ma.kota" should be subtopic of "ala".
      if (level <= subscription.level &&
          text::starts_with(topic, subscription.topic)) {

        std::string log;
        for (auto c: subscription.format) {
          switch (c) {
            case 'T':
              log += '[';
              log += topic;
              log += "] ";
              break;
            case 'L':
              log += '[';
              log += log_level_name(level);
              log += "] ";
              break;
            case 'D':
              log += '[';
              log += detail::date_time_string();
              log += "] ";
              break;
            default:
              continue;
          }
        }
        log += message;
        *subscription.stream << log;
      }
    }
  }

private:
  LogMaster() = default;

  struct SubscriptionEntry {
    std::string topic;
    log_level level;
    std::string format;
    std::ostream* stream;
  };

  std::vector<SubscriptionEntry> subscriptions_;
};

LogMaster& log_master() {
  static LogMaster logMaster;
  return logMaster;
}

class Logger {
public:
  Logger(std::string topic):
      topic_(std::move(topic)) { }

  template <typename... Args>
  void log(log_level level, const char* format, const Args&... args) {
    std::ostringstream stream;
    print(stream, format, args...);
    log_master().log(topic_, level, stream.str());
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

protected:
  Logger() = default;

private:
  std::string topic_;
};

} // namespace logging
} // namespace pcl
