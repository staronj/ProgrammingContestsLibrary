// Jakub Staroń, 2016-2017

#include "data_structures/van_emde_boas_set.h"
#include "io.h"

using namespace pcl;

std::string PrettyPrintSize(uint64 size) {
  std::ostringstream stream;
  stream << std::fixed;
  stream.precision(2);

  if (size < 1024)
    stream << size << " B";
  else if (size < 1024 * 1024)
    stream << (double) size / 1024 << " KB";
  else if (size < 1024 * 1024 * 1024)
    stream << (double) size / (1024 * 1024) << " MB";
  else
    stream << (double) size / (1024 * 1024 * 1024) << " GB";

  return stream.str();
}

template<uint32 N>
void print_all() {
  print_all<N - 1>();
  auto size = PrettyPrintSize(VanEmdeBoasSet<N>::sizeOfTree());
  print("VanEmdeBoasTree<%0> occupies %1.", N, size);
}

template<>
void print_all<2>() { }

int main() {
  print_all<32>();
}
