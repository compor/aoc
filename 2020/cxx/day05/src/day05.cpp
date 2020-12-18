//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

using ull = unsigned long long;
using ticket_t = std::string;

std::pair<ull, ull> split(bool upper, ull lo, ull r) {
  r /= 2;
  return upper ? std::make_pair(lo + r, r) : std::make_pair(lo, r);
}

ull seatid(const std::string &t) {
  ull rlo = 0, rr = 128;
  ull clo = 0, cr = 8;
  for (auto c : t) {
    if (c == 'F' or c == 'B') {
      auto [lo, r] = split(c == 'B', rlo, rr);
      rlo = lo;
      rr = r;
    } else {
      auto [lo, r] = split(c == 'R', clo, cr);
      clo = lo;
      cr = r;
    }
  }

  return rlo * 8 + clo;
}

int main(int argc, char *argv[]) {
  std::ifstream fin;
  fin.open(argv[1]);

  std::string inputval;
  std::vector<ticket_t> input;

  while (std::getline(fin, inputval)) {
    input.push_back(inputval);
  }

  ull max = 0;
  for (auto c : input) {
    max = std::max(max, seatid(c));
  }

  std::cout << max << std::endl;

  return EXIT_SUCCESS;
}
