// Jakub Staroń, 2016
//
// Solution to SPOJ problem, see
// http://www.spoj.com/problems/LCMSUM/

#include "headers.h"
#include "numeric/number_theory.h"
#include "logger.h"
#include "io.h"

using namespace lib;

logging::Logger& logger = logging::get_logger("main");

/**
 * Returns vector with values of Euler's totient function
 * for k = 0,1,...n-1
 */
std::vector<uint32> TotientFunctionValues(uint32 N) {
  std::vector<uint32> result(N, 1);
  result[0] = 0;

  const auto primes = numeric::PrimeNumbers(N);
  for (uint64 p: primes) {
    for (uint64 q = p; q < N; q *= p) {
      result[q] = (q / p) * (p - 1);
      for (uint64 i = 2; i * q < N; i++) {
        if (divides(p, i))
          continue;

        result[i * q] *= result[q];
      }
    }
  }

  return result;
}

constexpr uint32 kMaxN = 1000 * 1000;

class Application {
public:
  Application() {
    auto totient = TotientFunctionValues(kMaxN + 1);

    sum.assign(kMaxN + 1, 0);
    for (auto d: range<uint64>(1, kMaxN + 1)) {
      for (uint64 i = 1; i * d <= kMaxN; ++i) {
        sum[i * d] += (d * totient[d]);
      }
    }
  }

  uint64 Result(uint32 N) {
    return (sum[N] + 1) * N / 2u;
  }

  void Run() {
    auto input = ReadSequence<uint32>(std::cin, T);
    iterate(input, [this](uint32 N) {
      logger.info("N = %0", N);
      logger.info("sum[%0] = %1", N, sum[N]);
      print("%0", Result(N));
    });
  }

  void LoadData() {
    read(std::cin, T);
    logger.info("T = %0", T);
  }

private:
  std::vector<uint64> sum;
  uint32 T;
};

int main(int argc, const char* argv[]) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  logger.set_log_level(logging::log_level::none);
  logger.add_stream(&std::cerr, logging::log_level::debug);
  Application application;
  application.LoadData();
  application.Run();
  return 0;
}
