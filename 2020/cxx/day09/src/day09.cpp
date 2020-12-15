//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <utility>
#include <vector>

using ull = unsigned long long;

int main(int argc, char *argv[]) {
  std::ifstream fin;
  fin.open(argv[1]);

  std::string inputval;
  std::vector<ull> input;
  std::set<ull> p;
  const ull n = 25; // preamble length
  ull val = 0;

  ull we = 0;
  ull wb = 0;
  while (fin >> inputval) {
    val = std::stoull(inputval);

    if (we++ < n) {
      input.push_back(val);
      p.insert(val);
    } else {
      if (std::any_of(input.begin(), input.end(),
                      [val, &p](const auto &e) { return p.count(val - e); })) {
        p.erase(input[wb++]);
        input.push_back(val);
        p.insert(val);
      } else {
        break;
      }
    }
  }

  std::cout << val << std::endl;

  ull we2 = 0;
  ull wb2 = 0;
  for (auto i = 0u; i < we; ++i) {
    wb2 = i;
    we2 = i + 1;
    ull sum = input[wb2] + input[we2];
    while (sum < val) {
      sum += input[++we2];
    }

    if (sum == val) {
      break;
    }
  }

  std::sort(&input[wb2], &input[we2]);

  std::cout << input[wb2] + input[we2] << std::endl;

  return EXIT_SUCCESS;
}
