// Jakub Staroń, 2016-2017

#include "headers.h"
#include "iterators.h"
#include "io.h"
#include "logger.h"

using namespace pcl;

logging::Logger logger("main");

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
  logging::log_master().subscribe("main", logging::log_level::debug, "L", &std::cerr);
  Application application;
  application.LoadData();
  application.Run();
  return 0;
}
