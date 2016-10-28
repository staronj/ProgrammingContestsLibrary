// Jakub Staroń, 2016

#include "headers.h"
#include "iterators.h"
#include "io.h"
#include "logger.h"

using namespace lib;

logging::Logger& logger = logging::get_logger("main");

class Application {
public:
  Application() {

  }

  void Run() {

  }

  void LoadData() {

  }

private:

};

int main(int argc, const char* argv[]) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  logger.set_log_level(logging::log_level::debug);
  logger.add_stream(&std::cerr, logging::log_level::debug);
  Application application;
  application.LoadData();
  application.Run();
  return 0;
}
