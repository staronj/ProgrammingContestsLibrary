#pragma once
// Jakub Staroń, 2016

#include "io.h"

namespace lib {
namespace logger {

class Logger {


};

Logger& get_logger(const std::string& name) {
  static std::map<std::string, Logger> loggers;
  auto it = loggers.find(name);

  if (it == loggers.end()) {
    it = loggers.emplace(name, Logger());
  }
  return it->second;
}

} // namespace logger
} // namespace lib
