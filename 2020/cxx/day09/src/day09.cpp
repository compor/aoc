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
  std::vector<ull> window;
  std::set<ull> p;
  const ull n = 25; // preamble length
  ull val = 0;

  ull we = 0;
  ull wb = 0;
  while (fin >> inputval) {
    val = std::stoull(inputval);

    if (we++ < n) {
      window.push_back(val);
      p.insert(val);
    } else {
      if (std::any_of(window.begin(), window.end(),
                      [val, &p](const auto &e) { return p.count(val - e); })) {
        p.erase(window[wb++]);
        window.push_back(val);
        p.insert(val);
      } else {
        break;
      }
    }
  }

  std::cout << val << std::endl;

  return EXIT_SUCCESS;
}
